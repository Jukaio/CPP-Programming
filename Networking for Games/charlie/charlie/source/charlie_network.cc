// charlie_network.cc

#include "charlie_network.hpp"
#include "charlie_protocol.hpp"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>

#include <cstdio>
#include <cstdarg>

namespace charlie {
	namespace network {
		static Service* g_service = nullptr;

		// static 
		int32 Error::get_last()
		{
			return WSAGetLastError();
		}

		bool Error::is_critical(const int32 error)
		{
			if (error == 0 ||
				error == WSAEWOULDBLOCK ||
				error == WSAECONNRESET)
			{
				return false;
			}

			return true;
		}

		// static 
		bool IPAddress::local_addresses(DynamicArray<IPAddress>& addresses)
		{
			DWORD size = 0;
			GetAdaptersAddresses(AF_INET,
				GAA_FLAG_INCLUDE_PREFIX,
				NULL,
				NULL,
				&size);

			IP_ADAPTER_ADDRESSES* adapter_addresses = (IP_ADAPTER_ADDRESSES*)calloc(1, size);
			GetAdaptersAddresses(AF_INET,
				GAA_FLAG_INCLUDE_PREFIX,
				NULL,
				adapter_addresses,
				&size);

			for (IP_ADAPTER_ADDRESSES* iter = adapter_addresses; iter != NULL; iter = iter->Next) {
				if (iter->OperStatus == IfOperStatusUp && (iter->IfType == IF_TYPE_ETHERNET_CSMACD || iter->IfType == IF_TYPE_IEEE80211)) {
					for (IP_ADAPTER_UNICAST_ADDRESS* ua = iter->FirstUnicastAddress; ua != NULL; ua = ua->Next) {
						char addrstr[1024] = {};
						getnameinfo(ua->Address.lpSockaddr, ua->Address.iSockaddrLength, addrstr, sizeof(addrstr), NULL, 0, NI_NUMERICHOST);
						if (ua->Address.lpSockaddr->sa_family == AF_INET) {
							sockaddr_in ai = *(sockaddr_in*)ua->Address.lpSockaddr;
							IPAddress address;
							address.host_ = ntohl(ai.sin_addr.s_addr);
							address.port_ = ntohs(ai.sin_port);
							addresses.push_back(address);
						}
					}
				}
			}

			free(adapter_addresses);

			return !addresses.empty();
		}

		bool IPAddress::dns_lookup(const char* dns, DynamicArray<IPAddress>& addresses)
		{
			ADDRINFO* query_result = NULL;
			ADDRINFO hint = {};
			hint.ai_family = AF_INET;
			hint.ai_socktype = SOCK_DGRAM;
			bool result = getaddrinfo(dns, NULL, &hint, &query_result) == 0;
			if (result) {
				ADDRINFO* iter = query_result;
				while (iter) {
					sockaddr_in addrin = *(sockaddr_in*)iter->ai_addr;
					IPAddress address;
					address.host_ = ntohl(addrin.sin_addr.s_addr);
					address.port_ = ntohs(addrin.sin_port);
					addresses.push_back(address);
					iter = iter->ai_next;
				}
			}

			freeaddrinfo(query_result);

			return !addresses.empty();
		}

		IPAddress::IPAddress()
			: host_(ANY_HOST)
			, port_(0)
		{
		}

		IPAddress::IPAddress(const uint32 host,
			const uint16 port)
			: host_(host)
			, port_(port)
		{
		}

		IPAddress::IPAddress(const uint8 a,
			const uint8 b,
			const uint8 c,
			const uint8 d,
			const uint16 port)
			: host_(0)
			, port_(port)
		{
			host_ |= ((uint32)a) << 24;
			host_ |= ((uint32)b) << 16;
			host_ |= ((uint32)c) << 8;
			host_ |= ((uint32)d);
		}

		bool IPAddress::operator==(const IPAddress& rhs) const
		{
			return host_ == rhs.host_ && port_ == rhs.port_;
		}

		bool IPAddress::operator!=(const IPAddress& rhs) const
		{
			return host_ != rhs.host_ || port_ != rhs.port_;
		}

		const char* IPAddress::as_string() const
		{
			static char string[64] = {};
			sprintf_s(string,
				sizeof(string),
				"%d.%d.%d.%d:%d",
				(host_ >> 24) & 0xff,
				(host_ >> 16) & 0xff,
				(host_ >> 8) & 0xff,
				(host_ >> 0) & 0xff,
				port_);
			return string;
		}

		// static 
		bool UDPSocket::get_address(const UDPSocket & socket, IPAddress & address)
		{
			if (!socket.is_valid()) {
				return false;
			}

			sockaddr_in name = {};
			int name_size = sizeof(name);
			if (getsockname(socket.id_, (sockaddr*)& name, &name_size) < 0) {
				return false;
			}

			address.host_ = ntohl(name.sin_addr.s_addr);
			address.port_ = ntohs(name.sin_port);

			return true;
		}

		UDPSocket::UDPSocket()
			: id_(INVALID_SOCKET)
		{
		}

		bool UDPSocket::is_valid() const
		{
			return id_ != INVALID_SOCKET;
		}

		bool UDPSocket::open()
		{
			return open({});
		}

		bool UDPSocket::open(const IPAddress & address)
		{
			if (is_valid()) {
				close();
			}

			id_ = ::socket(AF_INET, SOCK_DGRAM, 0);
			if (id_ == INVALID_SOCKET) {
				return false;
			}

			sockaddr_in local = {};
			local.sin_family = AF_INET;
			local.sin_port = htons(address.port_);
			local.sin_addr.s_addr = htonl(address.host_);
			if (bind(id_, (const sockaddr*)& local, sizeof(local)) == SOCKET_ERROR) {
				return false;
			}

			u_long non_blocking = 1;
			if (ioctlsocket(id_, FIONBIO, &non_blocking) == SOCKET_ERROR) {
				return false;
			}

			return true;
		}

		void UDPSocket::close()
		{
			if (is_valid()) {
				::closesocket(id_);
				id_ = INVALID_SOCKET;
			}
		}

		bool UDPSocket::send(const IPAddress & address, const uint8 * data, const int32 length)
		{
			if (!is_valid()) {
				return false;
			}

			sockaddr_in remote = {};
			remote.sin_family = AF_INET;
			remote.sin_port = htons(address.port_);
			remote.sin_addr.s_addr = htonl(address.host_);
			int s = ::sendto(id_, (const char*)data, length, 0, (const sockaddr*)& remote, sizeof(remote));
			if (s == SOCKET_ERROR) {
				return false;
			}

			return true;
		}

		bool UDPSocket::receive(IPAddress & address, uint8 * data, int32 & length)
		{
			if (!is_valid()) {
				return false;
			}

			int remote_size = sizeof(sockaddr_in);
			sockaddr_in remote = {};
			int r = ::recvfrom(id_, (char*)data, length, 0, (sockaddr*)& remote, &remote_size);
			if (r == SOCKET_ERROR) {
				return false;
			}

			length = r;
			address.host_ = ntohl(remote.sin_addr.s_addr);
			address.port_ = ntohs(remote.sin_port);

			return true;
		}

		NetworkStream::NetworkStream()
			: length_(sizeof(buffer_))
			//         , buffer_{}
		{
		}

		void NetworkStream::reset()
		{
			length_ = sizeof(buffer_);
		}

		NetworkStreamWriter::NetworkStreamWriter(NetworkStream & stream)
			: stream_(stream)
			, at_(stream.buffer_)
		{
		}

		int32 NetworkStreamWriter::length() const
		{
			return static_cast<int32>(at_ - data());
		}

		uint8* NetworkStreamWriter::data() const
		{
			return stream_.buffer_;
		}

		namespace {
			template <typename T>
			inline bool can_write(const NetworkStreamWriter& writer, const T& value)
			{
				return (writer.at_ - writer.data()) + sizeof(T) <= sizeof(writer.stream_.buffer_);
			}

			inline bool can_write_bytes(const NetworkStreamWriter & writer, const uint64 length)
			{
				return (writer.at_ - writer.data()) + length <= sizeof(writer.stream_.buffer_);
			}

			inline void update_base_stream_length(NetworkStreamWriter & writer)
			{
				writer.stream_.length_ = writer.length();
			}
		} // !anon

		bool NetworkStreamWriter::serialize(const float& value)
		{
			if (!can_write(*this, value)) { return false; }
			union {
				float  f_;
				uint32 u_;
			} data = { value };
			return serialize(data.u_);
		}

		bool NetworkStreamWriter::serialize(const double& value)
		{
			if (!can_write(*this, value)) { return false; }
			union {
				double f_;
				uint64 u_;
			} data = { value };
			return serialize(data.u_);
		}

		bool NetworkStreamWriter::serialize(const uint8 & value)
		{
			if (!can_write(*this, value)) { return false; }
			at_[0] = value;
			at_ += sizeof(value);
			update_base_stream_length(*this);
			return true;
		}

		bool NetworkStreamWriter::serialize(const  int8 & value)
		{
			if (!can_write(*this, value)) { return false; }
			at_[0] = value;
			at_ += sizeof(value);
			update_base_stream_length(*this);
			return true;
		}

		bool NetworkStreamWriter::serialize(const uint16 & value)
		{
			if (!can_write(*this, value)) { return false; }
			at_[0] = (value >> 8) & 0xff;
			at_[1] = (value) & 0xff;
			at_ += sizeof(value);
			update_base_stream_length(*this);
			return true;
		}

		bool NetworkStreamWriter::serialize(const  int16 & value)
		{
			if (!can_write(*this, value)) { return false; }
			at_[0] = (value >> 8) & 0xff;
			at_[1] = (value) & 0xff;
			at_ += sizeof(value);
			update_base_stream_length(*this);
			return true;
		}

		bool NetworkStreamWriter::serialize(const uint32 & value)
		{
			if (!can_write(*this, value)) { return false; }
			at_[0] = (value >> 24) & 0xff;
			at_[1] = (value >> 16) & 0xff;
			at_[2] = (value >> 8) & 0xff;
			at_[3] = (value) & 0xff;
			at_ += sizeof(value);
			update_base_stream_length(*this);
			return true;
		}

		bool NetworkStreamWriter::serialize(const  int32 & value)
		{
			if (!can_write(*this, value)) { return false; }
			at_[0] = (value >> 24) & 0xff;
			at_[1] = (value >> 16) & 0xff;
			at_[2] = (value >> 8) & 0xff;
			at_[3] = (value) & 0xff;
			at_ += sizeof(value);
			update_base_stream_length(*this);
			return true;
		}

		bool NetworkStreamWriter::serialize(const uint64 & value)
		{
			if (!can_write(*this, value)) { return false; }
			at_[0] = (value >> 56) & 0xff;
			at_[1] = (value >> 48) & 0xff;
			at_[2] = (value >> 40) & 0xff;
			at_[3] = (value >> 32) & 0xff;
			at_[4] = (value >> 24) & 0xff;
			at_[5] = (value >> 16) & 0xff;
			at_[6] = (value >> 8) & 0xff;
			at_[7] = (value) & 0xff;
			at_ += sizeof(value);
			update_base_stream_length(*this);
			return true;
		}

		bool NetworkStreamWriter::serialize(const  int64 & value)
		{
			if (!can_write(*this, value)) { return false; }
			at_[0] = (value >> 56) & 0xff;
			at_[1] = (value >> 48) & 0xff;
			at_[2] = (value >> 40) & 0xff;
			at_[3] = (value >> 32) & 0xff;
			at_[4] = (value >> 24) & 0xff;
			at_[5] = (value >> 16) & 0xff;
			at_[6] = (value >> 8) & 0xff;
			at_[7] = (value) & 0xff;
			at_ += sizeof(value);
			update_base_stream_length(*this);
			return true;
		}

		bool NetworkStreamWriter::serialize(const uint64 length, const uint8 * values)
		{
			if (!can_write_bytes(*this, length)) { return false; }
			memcpy(at_, values, length);
			at_ += length;
			update_base_stream_length(*this);
			return true;
		}

		bool NetworkStreamWriter::serialize(const uint64 length, const char* values)
		{
			if (!can_write_bytes(*this, length)) { return false; }
			memcpy(at_, values, length);
			at_ += length;
			update_base_stream_length(*this);
			return true;
		}

		NetworkStreamReader::NetworkStreamReader(const NetworkStream & stream)
			: stream_(stream)
			, at_(stream.buffer_)
		{
		}

		uint8 NetworkStreamReader::peek() const
		{
			return at_[0];
		}

		int32 NetworkStreamReader::length() const
		{
			return static_cast<int32>(stream_.length_);
		}

		int32 NetworkStreamReader::position() const
		{
			return static_cast<int32>(at_ - data());
		}

		const uint8* NetworkStreamReader::data() const
		{
			return stream_.buffer_;
		}

		namespace {
			template <typename T>
			inline bool can_read(const NetworkStreamReader& reader, const T& value)
			{
				return (reader.at_ - reader.data()) + sizeof(T) <= reader.stream_.length_;
			}

			inline bool can_read_bytes(const NetworkStreamReader & reader, const uint64 length)
			{
				return (reader.at_ - reader.data()) + length <= reader.stream_.length_;
			}
		} // !anon

		bool NetworkStreamReader::serialize(float& value)
		{
			if (!can_read(*this, value)) { return false; }

			union {
				float  f_;
				uint32 u_;
			} data = {};

			if (!serialize(data.u_)) {
				return false;
			}

			value = data.f_;
			return true;
		}

		bool NetworkStreamReader::serialize(double& value)
		{
			if (!can_read(*this, value)) { return false; }
			union {
				double f_;
				uint64 u_;
			} data = {};

			if (!serialize(data.u_)) {
				return false;
			}

			value = data.f_;
			return true;
		}

		bool NetworkStreamReader::serialize(uint8 & value)
		{
			if (!can_read(*this, value)) { return false; }
			value = at_[0];
			at_ += sizeof(value);
			return true;
		}

		bool NetworkStreamReader::serialize(int8 & value)
		{
			if (!can_read(*this, value)) { return false; }
			value = at_[0];
			at_ += sizeof(value);
			return true;
		}

		bool NetworkStreamReader::serialize(uint16 & value)
		{
			if (!can_read(*this, value)) { return false; }
			value = ((uint16)at_[0]) << 8;
			value |= ((uint16)at_[1]);
			at_ += sizeof(value);
			return true;
		}

		bool NetworkStreamReader::serialize(int16 & value)
		{
			if (!can_read(*this, value)) { return false; }
			value = ((int16)at_[0]) << 8;
			value |= ((int16)at_[1]);
			at_ += sizeof(value);
			return true;
		}

		bool NetworkStreamReader::serialize(uint32 & value)
		{
			if (!can_read(*this, value)) { return false; }
			value = ((uint32)at_[0]) << 24;
			value |= ((uint32)at_[1]) << 16;
			value |= ((uint32)at_[2]) << 8;
			value |= ((uint32)at_[3]);
			at_ += sizeof(value);
			return true;
		}

		bool NetworkStreamReader::serialize(int32 & value)
		{
			if (!can_read(*this, value)) { return false; }
			value = ((int32)at_[0]) << 24;
			value |= ((int32)at_[1]) << 16;
			value |= ((int32)at_[2]) << 8;
			value |= ((int32)at_[3]);
			at_ += sizeof(value);
			return true;
		}

		bool NetworkStreamReader::serialize(uint64 & value)
		{
			if (!can_read(*this, value)) { return false; }
			value = ((uint64)at_[0]) << 56;
			value |= ((uint64)at_[1]) << 48;
			value |= ((uint64)at_[2]) << 40;
			value |= ((uint64)at_[3]) << 32;
			value |= ((uint64)at_[4]) << 24;
			value |= ((uint64)at_[5]) << 16;
			value |= ((uint64)at_[6]) << 8;
			value |= ((uint64)at_[7]);
			at_ += sizeof(value);
			return true;
		}

		bool NetworkStreamReader::serialize(int64 & value)
		{
			if (!can_read(*this, value)) { return false; }
			value = ((int64)at_[0]) << 56;
			value |= ((int64)at_[1]) << 48;
			value |= ((int64)at_[2]) << 40;
			value |= ((int64)at_[3]) << 32;
			value |= ((int64)at_[4]) << 24;
			value |= ((int64)at_[5]) << 16;
			value |= ((int64)at_[6]) << 8;
			value |= ((int64)at_[7]);
			at_ += sizeof(value);
			return true;
		}

		bool NetworkStreamReader::serialize(const uint64 length, uint8 * values)
		{
			if (!can_read_bytes(*this, length)) { return false; }
			memcpy(values, at_, length);
			at_ += length;
			return true;
		}

		bool NetworkStreamReader::serialize(const uint64 length, char* values)
		{
			if (!can_read_bytes(*this, length)) { return false; }
			memcpy(values, at_, length);
			at_ += length;
			return true;
		}

		Connection::Connection()
			: state_(State::Invalid)
			, key_(0)
			, challenge_(0)
			, sequence_(0)
			, acknowledge_(0)
			, acknowledge_bits_(0)
			, rejection_reason_(RejectedReason::REJECT_REASON_UNKNOWN)
			, disconnect_counter_(0)
			, listener_(nullptr)
		{
		}

		bool Connection::is_valid() const
		{
			return state_ != State::Invalid;
		}

		bool Connection::is_endpoint(const IPAddress & address) const
		{
			return address_ == address;
		}

		bool Connection::is_connected() const
		{
			return state_ == State::Connected;
		}

		bool Connection::is_disconnected() const
		{
			return state_ == State::Disconnected;
		}

		bool Connection::is_requesting() const
		{
			return state_ == State::SendingRequest;
		}

		bool Connection::is_responding() const
		{
			return state_ == State::SendingResponse;
		}

		bool Connection::is_challenging() const
		{
			return state_ == State::SendingChallenge;
		}

		bool Connection::is_disconnecting() const
		{
			return state_ == State::Disconnecting;
		}

		bool Connection::has_timedout(const Time & time) const
		{
			// todo: replace hard coded timeout limit
			return (time - last_received_time_) >= Time(10.0);
		}

		Time Connection::latency() const
		{
			return round_trip_time_ / 2;
		}

		Time Connection::round_trip_time() const
		{
			return round_trip_time_;
		}

		void Connection::connect(const IPAddress & address)
		{
			set_state(State::SendingRequest);
			set_address(address);

			assert(g_service);
			g_service->add_pending_connection(this);
		}

		void Connection::disconnect()
		{
			set_state(State::Disconnecting);
			disconnect_counter_ = 5;
		}

		void Connection::on_rejected(const uint8 reason)
		{
			set_state(State::Disconnected);
			rejection_reason_ = reason;
		}

		void Connection::on_timedout()
		{
			set_state(State::Timedout);
		}

		void Connection::on_established()
		{
			set_state(State::Connected);
		}

		void Connection::on_disconnected()
		{
			set_state(State::Disconnected);
		}

		void Connection::invalidate()
		{
			address_ = {};
			state_ = State::Invalid;
			key_ = 0;
			challenge_ = 0;
			sequence_ = 0;
			acknowledge_ = 0;
			acknowledge_bits_ = 0;
			rejection_reason_ = 0;
			disconnect_counter_ = 5;
			last_sent_time_ = {};
			last_received_time_ = {};
			connection_established_time_ = {};
			round_trip_time_ = {};
			listener_ = nullptr;
		}

		void Connection::set_state(const State state)
		{
			// todo: from state
			switch (state_) {
			case State::Invalid: break;
			default: break;
			}

			// todo: to state
			switch (state) {
			case State::Invalid: break;
			default: break;
			}

			state_ = state;
		}

		void Connection::set_address(const IPAddress & address)
		{
			address_ = address;
		}

		void Connection::set_key(const uint64 key)
		{
			key_ = key;
		}

		void Connection::set_challenge(const uint64 challenge)
		{
			challenge_ = challenge;
		}

		void Connection::set_sent_time(const Time & time)
		{
			last_sent_time_ = time;
		}

		void Connection::set_received_time(const Time & time)
		{
			last_received_time_ = time;
		}

		void Connection::set_connected_time(const Time & time)
		{
			connection_established_time_ = time;
		}

		void Connection::set_listener(IConnectionListener * listener)
		{
			listener_ = listener;
		}

		namespace {
			bool is_sequence_newer(const uint16 a, const uint16 b) {
				return ((a > b) && ((a - b) <= 0x8000)) ||
					((a < b) && ((b - a) > 0x8000));
			}

			bool is_sequence_older(const uint16 a, const uint16 b) {
				return !is_sequence_newer(b, a);
			}

			int32 sequence_difference(const uint16 a, const uint16 b)
			{
				int32 x = a;
				int32 y = b;
				if (abs(x - y) >= 0x8000) {
					if (x > y) {
						y += 0x8000;
					}
					else {
						x += 0x8000;
					}
				}

				return x - y;
			}
		} // !anon

		void Connection::receive(NetworkStreamReader& reader)
		{
			set_received_time(Time::now());

			ProtocolDataPacket packet;
			if (!packet.read(reader)) {
				assert(!"data packet read failed!");
				return;
			}

			// note: ignore old data packets
			if (is_sequence_older(acknowledge_, packet.sequence_)) {
				printf("WRN: old sequence - ack: %d seq: %d\n", acknowledge_, packet.sequence_);
				return;
			}

			int32 distance = sequence_difference(packet.sequence_, acknowledge_);
			acknowledge_ = packet.sequence_;
			if (distance >= 32) {
				acknowledge_bits_ = 0;
			}
			else {
				acknowledge_bits_ >>= distance;
				acknowledge_bits_ |= (1 << 31);
			}

			const Time timestamp = round_trip_buffer_[packet.acknowledge_ % 64];
			const Time ticks(int64(packet.ticks_));
			round_trip_time_ = last_received_time_ - timestamp - ticks;

			//printf("NFO: seq: %d ack: %d (%08X) rtt: %3.3fms [%d,%u]\n",
			//       sequence_,
			//       acknowledge_,
			//       acknowledge_bits_,
			//       round_trip_time_.as_milliseconds(),
			//       packet.acknowledge_,
			//       packet.ticks_);

			if (listener_) {
				listener_->on_acknowledge(this, acknowledge_);
				// todo: parse bits and let listener know
			}

			// note: listener read the data
			if (listener_) {
				listener_->on_receive(this, reader);
			}
		}

		void Connection::send()
		{
			const Time now = Time::now();
			round_trip_buffer_[sequence_ % 64] = now;

			NetworkStream stream;
			NetworkStreamWriter writer(stream);

			uint32 ticks = static_cast<uint32>((now - last_received_time_).as_ticks());
			ProtocolDataPacket packet(sequence_,
				acknowledge_,
				acknowledge_bits_,
				ticks);
			if (!packet.write(writer)) {
				assert(!"data packet write failed!");
			}

			// note: listener pack data
			if (listener_) {
				listener_->on_send(this, sequence_, writer);
			}
			sequence_++;

			assert(g_service);
			g_service->send_stream(this, stream);
		}

		ConnectionPool::ConnectionPool(const int32 capacity)
			: capacity_(capacity)
			, connection_count_(0)
			, connections_(new Connection[capacity])
		{
		}

		ConnectionPool::~ConnectionPool()
		{
			delete[] connections_;
		}

		void ConnectionPool::resize(const int32 capacity)
		{
			if (capacity_ < capacity) {
				delete[] connections_;
				capacity_ = capacity;
				connection_count_ = 0;
				connections_ = new Connection[capacity];
			}
		}

		Connection* ConnectionPool::create()
		{
			assert(connection_count_ < capacity_);
			connection_count_++;

			for (int32 index = 0; index < capacity_; index++) {
				if (!connections_[index].is_valid()) {
					connections_[index].set_state(Connection::State::Disconnected);
					return &connections_[index];
				}
			}

			return nullptr;
		}

		void ConnectionPool::release(Connection * connection)
		{
			assert(connection >= connections_ && connection <= (connections_ + capacity_));
			connection->invalidate();

			assert(connection_count_ > 0);
			connection_count_--;
		}

		Service::Service()
			: initialized_(false)
			, send_rate_(1.0 / 10)
			, allow_connections_(false)
			, connection_limit_(8)
			, connection_pool_(connection_limit_)
		{
			assert(!g_service);
			g_service = this;

			WSADATA data = {};
			WSAStartup(MAKEWORD(2, 2), &data);
		}

		Service::~Service()
		{
			WSACleanup();
		}

		bool Service::initialize(const IPAddress & address)
		{
			if (!socket_.open(address)) {
				int error_code = Error::get_last();
				return false;
			}

			initialized_ = true;

			return true;
		}

		void Service::shutdown()
		{
			if (socket_.is_valid()) {
				socket_.close();
			}
			initialized_ = false;
		}

		void Service::update()
		{
			// note: deducted using hard science!
			int32 receive_limiter = 20;

			IPAddress address;
			NetworkStream stream;
			while (receive_limiter >= 0 && socket_.receive(address, stream.buffer_, stream.length_)) {
				NetworkStreamReader reader(stream);
				// note: allow_connections_ == true indicates 'server' mode
				if (allow_connections_) {
					switch (reader.peek()) {
					case PROTOCOL_PACKET_REQUEST:
						handle_connection_request(address, reader);
						break;
					case PROTOCOL_PACKET_CHALLENGE:
						handle_connection_challenge(address, reader);
						break;
					case PROTOCOL_PACKET_RESPONSE:
						handle_connection_response(address, reader);
						break;
					case PROTOCOL_PACKET_REJECTED:
						handle_connection_rejected(address, reader);
						break;
					case PROTOCOL_PACKET_DATA:
						handle_connection_payload(address, reader);
						break;
					case PROTOCOL_PACKET_DISCONNECT:
						handle_connection_disconnect(address, reader);
						break;
					default:
						assert(!"invalid packet received!");
						break;
					}
				}
				else {
					switch (reader.peek()) {
					case PROTOCOL_PACKET_CHALLENGE:
						handle_connection_challenge(address, reader);
						break;
					case PROTOCOL_PACKET_REJECTED:
						handle_connection_rejected(address, reader);
						break;
					case PROTOCOL_PACKET_DATA:
						handle_connection_payload(address, reader);
						break;
					case PROTOCOL_PACKET_DISCONNECT:
						handle_connection_disconnect(address, reader);
						break;
					default:
						assert(!"invalid packet received!");
						break;
					}
				}

				receive_limiter--;
				stream.reset();
			}

			const int error_code = Error::get_last();
			if (Error::is_critical(error_code)) {
				assert(!"network critical error!");
			}

			const Time time = Time::now();
			for (auto& connection : pending_connections_) {
				if (allow_connections_) {
					if ((time - connection->last_sent_time_) >= Time(0.2)) {
						if (connection->is_challenging()) {
							send_connection_challenge(connection);
						}
					}
				}
				else {
					if ((time - connection->last_sent_time_) >= Time(0.2)) {
						if (connection->is_requesting()) {
							send_connection_request(connection);
						}
						else if (connection->is_responding()) {
							send_connection_response(connection);
						}
					}
				}
			}

			for (auto& connection : established_connections_) {
				if ((time - connection->last_sent_time_) >= send_rate_) {
					if (connection->is_connected()) {
						connection->send();
					}
					else if (connection->is_disconnecting()) {
						if (connection->disconnect_counter_ == 0) {
							printf("NFO: disconnect counter reached.\n");
							connection->set_state(Connection::State::Disconnected);
						}
						else {
							connection->disconnect_counter_--;
							send_connection_disconnect(connection);
						}
					}
				}
			}

			perform_periodic_timeout_check(Time::now());
		}

		void Service::set_send_rate(const Time & rate)
		{
			send_rate_ = rate;
		}

		void Service::set_allow_connections(const bool allow_connections)
		{
			allow_connections_ = allow_connections;
		}

		void Service::set_connection_limit(const int32 connection_limit)
		{
			assert(initialized_ == false);
			connection_limit_ = connection_limit;
			connection_pool_.resize(connection_limit);
		}

		void Service::add_service_listener(IServiceListener * listener)
		{
			connection_listeners_.push_back(listener);
		}

		void Service::remove_service_listener(IServiceListener * listener)
		{
			auto it = connection_listeners_.begin();
			while (it != connection_listeners_.end()) {
				if ((*it) == listener) {
					connection_listeners_.erase(it);
					break;
				}
				++it;
			}
		}

		void Service::notify_service_listeners(const Event event, Connection * connection)
		{
			for (auto& listener : connection_listeners_) {
				switch (event) {
				case Event::Timeout:
					listener->on_timeout(connection);
					break;
				case Event::Connect:
					listener->on_connect(connection);
					break;
				case Event::Disconnect:
					listener->on_disconnect(connection);
					break;
				}
			}
		}

		void Service::add_pending_connection(Connection * connection)
		{
			connection->set_key(random_());
			pending_connections_.push_back(connection);
		}

		void Service::remove_pending_connection(Connection * connection)
		{
			auto it = pending_connections_.begin();
			while (it != pending_connections_.end()) {
				if ((*it) == connection) {
					pending_connections_.erase(it);
					return;
				}
				++it;
			}

			assert(!"remove_pending_connection: not found!");
		}

		Connection* Service::find_pending_connection(const IPAddress & address) const
		{
			for (auto& connection : pending_connections_) {
				if (connection->is_endpoint(address)) {
					return connection;
				}
			}
			return nullptr;
		}

		void Service::add_established_connection(Connection * connection)
		{
			established_connections_.push_back(connection);
		}

		void Service::remove_established_connection(Connection * connection)
		{
			auto it = established_connections_.begin();
			while (it != established_connections_.end()) {
				if ((*it) == connection) {
					established_connections_.erase(it);
					return;
				}
				++it;
			}

			assert(!"remove_established_connection: not found!");
		}

		Connection* Service::find_established_connection(const IPAddress & address) const
		{
			for (auto& connection : established_connections_) {
				if (connection->is_endpoint(address)) {
					return connection;
				}
			}
			return nullptr;
		}

		void Service::handle_connection_request(const IPAddress & address, NetworkStreamReader & reader)
		{
			printf("NFO: + handle_connection_request from %s\n", address.as_string());

			if (established_connections_.size() >= connection_limit_) {
				printf("NFO: server connection limit reached (%d).\n", connection_limit_);

				NetworkStream stream;
				NetworkStreamWriter writer(stream);
				ProtocolRejectedPacket rejected(REJECT_REASON_SERVER_FULL);
				if (rejected.write(writer)) {
					send_stream(address, stream);
				}
				return;
			}

			ProtocolRequestPacket packet;
			if (!packet.read(reader)) {
				assert(!"handle_connection_request: could not read packet!");
			}

			if (packet.protocol_ != PROTOCOL_ID) {
				printf("NFO: invalid protocol - %u != %u", PROTOCOL_ID, packet.protocol_);

				NetworkStream stream;
				NetworkStreamWriter writer(stream);
				ProtocolRejectedPacket rejected(REJECT_REASON_PROTOCOL);
				if (rejected.write(writer)) {
					send_stream(address, stream);
				}
				return;
			}
			else if (packet.version_ != PROTOCOL_VERSION) {
				printf("NFO: invalid version - %u != %u", PROTOCOL_VERSION, packet.version_);

				NetworkStream stream;
				NetworkStreamWriter writer(stream);
				ProtocolRejectedPacket rejected(REJECT_REASON_VERSION);
				if (rejected.write(writer)) {
					send_stream(address, stream);
				}
				return;
			}

			Connection* connection = find_pending_connection(address);
			if (!connection) {
				printf("NFO: no matching pending connection found.\n");
				if (find_established_connection(address)) {
					printf("NFO: found established connection with address.\n");
					return;
				}

				connection = connection_pool_.create();
				if (!connection) {
					printf("WRN: no more connections in connection pool!\n");
					return;
				}

				printf("NFO: new pending connection created.\n");
				connection->set_address(address);
				connection->set_received_time(Time::now());
				add_pending_connection(connection);
			}

			if (connection->is_disconnected()) {
				printf("NFO: setting key and generating challenge.\n");
				connection->set_key(packet.key_);
				connection->set_challenge(random_());
				connection->set_received_time(Time::now());
				connection->set_state(Connection::State::SendingChallenge);
			}
		}

		void Service::handle_connection_challenge(const IPAddress & address, NetworkStreamReader & reader)
		{
			printf("NFO: + handle_connection_challenge from %s\n", address.as_string());
			Connection* connection = find_pending_connection(address);
			if (!connection) {
				printf("NFO: no matching pending connection found.\n");
				return;
			}

			ProtocolChallengePacket packet;
			if (!packet.read(reader)) {
				assert(!"handle_connection_challenge: could not read packet!");
			}

			if (connection->is_requesting()) {
				printf("NFO: setting challenge.\n");
				connection->set_challenge(packet.challenge_);
				connection->set_received_time(Time::now());
				connection->set_state(Connection::State::SendingResponse);
			}
		}

		void Service::handle_connection_response(const IPAddress & address, NetworkStreamReader & reader)
		{
			printf("NFO: + handle_connection_response from %s\n", address.as_string());
			Connection* connection = find_pending_connection(address);
			if (!connection) {
				printf("WRN: no matching pending connection found!\n");
				return;
			}

			ProtocolResponsePacket packet;
			if (!packet.read(reader)) {
				assert(!"handle_connection_response: could not read packet!");
			}

			if (connection->is_challenging()) {
				const uint64 correct = connection->key_ ^ connection->challenge_;
				if (packet.response_ != correct) {
					printf("NFO: invalid challenge - %llu <- %llu", packet.response_, correct);

					NetworkStream stream;
					NetworkStreamWriter writer(stream);
					ProtocolRejectedPacket rejected(REJECT_REASON_CHALLENGE);
					if (rejected.write(writer)) {
						send_stream(address, stream);
					}

					// todo: ...
					//connection->set_state(Connection::State::Rejected);
					return;
				}

				printf("NFO: challenge accepted, connection established.\n");
				remove_pending_connection(connection);
				add_established_connection(connection);

				const Time now = Time::now();
				connection->set_received_time(now);
				connection->set_connected_time(now);
				connection->on_established();
				notify_service_listeners(Event::Connect, connection);
			}
		}

		void Service::handle_connection_rejected(const IPAddress & address, NetworkStreamReader & reader)
		{
			printf("NFO: + handle_connection_rejected from %s\n", address.as_string());
			Connection* connection = find_pending_connection(address);
			if (!connection) {
				printf("WRN: no matching pending connection found!\n");
				return;
			}

			if (connection->is_requesting() || connection->is_responding()) {
				ProtocolRejectedPacket packet;
				if (!packet.read(reader)) {
					assert(!"handle_connection_rejected: could not read packet!");
				}

				printf("NFO: rejection reason #%d\n", packet.reason_);
				remove_pending_connection(connection);
				connection->on_rejected(packet.reason_);
			}
		}

		void Service::handle_connection_payload(const IPAddress & address, NetworkStreamReader & reader)
		{
			//printf("NFO: + handle_connection_payload from %s\n", address.as_string());
			Connection* connection = find_established_connection(address);
			if (!connection) {
				printf("WRN: no matching established connection found!\n");
				if (allow_connections_) {
					return;
				}
				else {
					printf("NFO: trying to find a pending connection.\n");
					connection = find_pending_connection(address);
					if (!connection) {
						printf("WRN: no pending connection found either!\n");
						return;
					}
				}
			}

			if (!allow_connections_ && connection->is_responding()) {
				printf("NFO: challenge accepted, connection established!\n");
				add_established_connection(connection);

				const Time now = Time::now();
				connection->set_received_time(now);
				connection->set_connected_time(now);
				connection->on_established();
				notify_service_listeners(Event::Connect, connection);
			}

			if (connection->is_connected()) {
				connection->set_received_time(Time::now());
				connection->receive(reader);
				if (allow_connections_) {

				}
			}
		}

		void Service::handle_connection_disconnect(const IPAddress & address, NetworkStreamReader & reader)
		{
			printf("NFO: + handle_connection_disconnect from %s\n", address.as_string());
			Connection* connection = find_established_connection(address);
			if (!connection) {
				printf("WRN: no matching established connection found!\n");
				printf("     might already be disconnected?\n");
				return;
			}

			if (connection->is_connected()) {
				printf("NFO: disconnect from %s.\n", connection->address_.as_string());
				remove_established_connection(connection);
				notify_service_listeners(Event::Disconnect, connection);
				connection->on_disconnected();
				connection->invalidate();
				if (allow_connections_) {
					connection_pool_.release(connection);
				}
			}
		}

		void Service::send_connection_request(Connection * connection)
		{
			if (!connection) {
				return;
			}

			NetworkStream stream;
			NetworkStreamWriter writer(stream);
			ProtocolRequestPacket packet(connection->key_);
			if (!packet.write(writer)) {
				assert(!"could not write request packet");
			}

			send_stream(connection, stream);
		}

		void Service::send_connection_response(Connection * connection)
		{
			if (!connection) {
				return;
			}

			NetworkStream stream;
			NetworkStreamWriter writer(stream);
			ProtocolResponsePacket packet(connection->key_ ^ connection->challenge_);
			if (!packet.write(writer)) {
				assert(!"could not write response packet");
			}

			send_stream(connection, stream);
		}

		void Service::send_connection_challenge(Connection * connection)
		{
			NetworkStream stream;
			NetworkStreamWriter writer(stream);
			ProtocolChallengePacket packet(connection->challenge_);
			if (!packet.write(writer)) {
				assert(!"could not write challenge packet");
			}

			send_stream(connection, stream);
		}

		void Service::send_connection_rejected(Connection * connection, const uint8 reason)
		{
			NetworkStream stream;
			NetworkStreamWriter writer(stream);
			ProtocolRejectedPacket packet(reason);
			if (!packet.write(writer)) {
				assert(!"could not write rejected packet");
			}

			send_stream(connection, stream);
		}

		void Service::send_connection_disconnect(Connection * connection)
		{
			NetworkStream stream;
			NetworkStreamWriter writer(stream);
			ProtocolDisconnectPacket packet;
			if (!packet.write(writer)) {
				assert(!"could not write disconnect packet");
			}

			send_stream(connection, stream);
		}

		void Service::send_stream(Connection * connection, const NetworkStream & stream)
		{
			if (!socket_.is_valid()) {
				return;
			}

			if (!connection || !connection->is_valid()) {
				return;
			}

			connection->set_sent_time(Time::now());
			if (!socket_.send(connection->address_, stream.buffer_, stream.length_)) {
				const int32 error_code = Error::get_last();
				if (Error::is_critical(error_code)) {
					// todo: handle socket error
					assert(false);
				}

				return;
			}
		}

		void Service::send_stream(const IPAddress & address, const NetworkStream & stream)
		{
			if (!socket_.is_valid()) {
				return;
			}

			if (!socket_.send(address, stream.buffer_, stream.length_)) {
				const int32 error_code = Error::get_last();
				if (Error::is_critical(error_code)) {
					// todo: handle socket error
					assert(false);
				}
			}
		}

		void Service::perform_periodic_timeout_check(const Time & time)
		{
			if ((time - last_timeout_check_) >= Time(0.1)) {
				last_timeout_check_ = time;

				for (auto it = pending_connections_.begin();
					it != pending_connections_.end();
					)
				{
					if ((*it)->has_timedout(time)) {
						printf("NFO: pending connection timeout from %s\n", (*it)->address_.as_string());
						(*it)->on_timedout();
						it = pending_connections_.erase(it);
					}
					else {
						++it;
					}
				}

				for (auto it = established_connections_.begin();
					it != established_connections_.end();
					)
				{
					Connection* connection = (*it);
					if (!connection->is_valid()) {
						it = established_connections_.erase(it);
						if (allow_connections_) {
							connection_pool_.release(connection);
						}
					}
					else if (connection->has_timedout(time)) {
						printf("NFO: established connection timeout from %s\n", connection->address_.as_string());
						notify_service_listeners(Event::Timeout, connection);
						connection->on_timedout();
						connection->invalidate();
						it = established_connections_.erase(it);
						if (allow_connections_) {
							connection_pool_.release(connection);
						}
					}
					else if (connection->is_disconnected()) {
						notify_service_listeners(Event::Disconnect, connection);
						connection->invalidate();
						it = established_connections_.erase(it);
						if (allow_connections_) {
							connection_pool_.release(connection);
						}
					}
					else {
						++it;
					}
				}
			}
		}
	} // !network
} // !charlie
