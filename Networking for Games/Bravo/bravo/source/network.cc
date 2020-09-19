// network.cc

#include "network.hpp"

#include <cstdio>
#include <cstdarg>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>

namespace bravo {
   // static 
   int32 Network::get_error()
   {
      return WSAGetLastError();
   }

   bool Network::is_critical(const int32 error)
   {
      if (error == 0 ||
          error == WSAEWOULDBLOCK ||
          error == WSAECONNRESET)
      {
         return false;
      }

      return true;
   }

   Network::Network()
   {
      WSADATA data = {};
      WSAStartup(MAKEWORD(2, 2), &data);
   }

   Network::~Network()
   {
      WSACleanup();
   }

   // static 
   bool IPAddress::local_addresses(DynamicArray<IPAddress> &addresses)
   {
      DWORD size = 0;
      GetAdaptersAddresses(AF_INET,
                           GAA_FLAG_INCLUDE_PREFIX,
                           NULL,
                           NULL,
                           &size);

      IP_ADAPTER_ADDRESSES *adapter_addresses = (IP_ADAPTER_ADDRESSES *)calloc(1, size);
      GetAdaptersAddresses(AF_INET,
                           GAA_FLAG_INCLUDE_PREFIX,
                           NULL,
                           adapter_addresses,
                           &size);

      for (IP_ADAPTER_ADDRESSES *iter = adapter_addresses; iter != NULL; iter = iter->Next) {
         if (iter->OperStatus == IfOperStatusUp && (iter->IfType == IF_TYPE_ETHERNET_CSMACD || iter->IfType == IF_TYPE_IEEE80211)) {
            for (IP_ADAPTER_UNICAST_ADDRESS *ua = iter->FirstUnicastAddress; ua != NULL; ua = ua->Next) {
               char addrstr[1024] = {};
               getnameinfo(ua->Address.lpSockaddr, ua->Address.iSockaddrLength, addrstr, sizeof(addrstr), NULL, 0, NI_NUMERICHOST);
               if (ua->Address.lpSockaddr->sa_family == AF_INET) {
                  sockaddr_in ai = *(sockaddr_in *)ua->Address.lpSockaddr;
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

   bool IPAddress::dns_lookup(const char *dns, DynamicArray<IPAddress> &addresses)
   {
      ADDRINFO *query_result = NULL;
      ADDRINFO hint = {};
      hint.ai_family = AF_INET;
      hint.ai_socktype = SOCK_DGRAM;
      bool result = getaddrinfo(dns, NULL, &hint, &query_result) == 0;
      if (result) {
         ADDRINFO *iter = query_result;
         while (iter) {
            sockaddr_in addrin = *(sockaddr_in *)iter->ai_addr;
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

   bool IPAddress::operator==(const IPAddress &rhs) const
   {
      return host_ == rhs.host_ && port_ == rhs.port_;
   }

   bool IPAddress::operator!=(const IPAddress &rhs) const
   {
      return host_ != rhs.host_ || port_ != rhs.port_;
   }

   const char *IPAddress::as_string() const
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
   bool UDPSocket::get_address(const UDPSocket &socket, IPAddress &address)
   {
      if (!socket.is_valid()) {
         return false;
      }

      sockaddr_in name = {};
      int name_size = sizeof(name);
      if (getsockname(socket.id_, (sockaddr *)&name, &name_size) < 0) {
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

   bool UDPSocket::open(const IPAddress &address)
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
      if (bind(id_, (const sockaddr *)&local, sizeof(local)) == SOCKET_ERROR) {
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

   bool UDPSocket::send(const IPAddress &address, const uint8 *data, const int32 length)
   {
      if (!is_valid()) {
         return false;
      }

      sockaddr_in remote = {};
      remote.sin_family = AF_INET;
      remote.sin_port = htons(address.port_);
      remote.sin_addr.s_addr = htonl(address.host_);
      int s = ::sendto(id_, (const char *)data, length, 0, (const sockaddr *)&remote, sizeof(remote));
      if (s == SOCKET_ERROR) {
         return false;
      }

      return true;
   }

   bool UDPSocket::receive(IPAddress &address, uint8 *data, int32 &length)
   {
      if (!is_valid()) {
         return false;
      }

      int remote_size = sizeof(sockaddr_in);
      sockaddr_in remote = {};
      int r = ::recvfrom(id_, (char *)data, length, 0, (sockaddr *)&remote, &remote_size);
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
      , buffer_{}
   {
   }

   NetworkStreamWriter::NetworkStreamWriter(NetworkStream &stream)
      : stream_(stream)
      , at_(stream.buffer_)
   {
   }

   int32 NetworkStreamWriter::length() const
   {
      return static_cast<int32>(at_ - data());
   }

   uint8 *NetworkStreamWriter::data() const
   {
      return stream_.buffer_;
   }

   namespace {
      template <typename T>
      inline bool can_write(const NetworkStreamWriter &writer, const T &value)
      {
         return (writer.at_ - writer.data()) + sizeof(T) <= sizeof(writer.stream_.buffer_);
      }

      inline bool can_write_bytes(const NetworkStreamWriter &writer, const uint64 length)
      {
         return (writer.at_ - writer.data()) + length <= sizeof(writer.stream_.buffer_);
      }

      inline void update_base_stream_length(NetworkStreamWriter &writer)
      {
         writer.stream_.length_ = writer.length();
      }
   } // !anon

   bool NetworkStreamWriter::serialize(const float &value)
   {
      if (!can_write(*this, value)) { return false; }
      union {
         float  f_;
         uint32 u_;
      } data = { value };
      return serialize(data.u_);
   }

   bool NetworkStreamWriter::serialize(const double &value)
   {
      if (!can_write(*this, value)) { return false; }
      union {
         double f_;
         uint64 u_;
      } data = { value };
      return serialize(data.u_);
   }

   bool NetworkStreamWriter::serialize(const uint8 &value)
   {
      if (!can_write(*this, value)) { return false; }
      at_[0] = value;
      at_ += sizeof(value);
      update_base_stream_length(*this);
      return true;
   }

   bool NetworkStreamWriter::serialize(const  int8 &value)
   {
      if (!can_write(*this, value)) { return false; }
      at_[0] = value;
      at_ += sizeof(value);
      update_base_stream_length(*this);
      return true;
   }

   bool NetworkStreamWriter::serialize(const uint16 &value)
   {
      if (!can_write(*this, value)) { return false; }
      at_[0] = (value >> 8) & 0xff;
      at_[1] = (value) & 0xff;
      at_ += sizeof(value);
      update_base_stream_length(*this);
      return true;
   }

   bool NetworkStreamWriter::serialize(const  int16 &value)
   {
      if (!can_write(*this, value)) { return false; }
      at_[0] = (value >> 8) & 0xff;
      at_[1] = (value) & 0xff;
      at_ += sizeof(value);
      update_base_stream_length(*this);
      return true;
   }

   bool NetworkStreamWriter::serialize(const uint32 &value)
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

   bool NetworkStreamWriter::serialize(const  int32 &value)
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

   bool NetworkStreamWriter::serialize(const uint64 &value)
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

   bool NetworkStreamWriter::serialize(const  int64 &value)
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

   bool NetworkStreamWriter::serialize(const uint64 length, const uint8 *values)
   {
      if (!can_write_bytes(*this, length)) { return false; }
      memcpy(at_, values, length);
      at_ += length;
      update_base_stream_length(*this);
      return true;
   }

   bool NetworkStreamWriter::serialize(const uint64 length, const char *values)
   {
      if (!can_write_bytes(*this, length)) { return false; }
      memcpy(at_, values, length);
      at_ += length;
      update_base_stream_length(*this);
      return true;
   }

   NetworkStreamReader::NetworkStreamReader(const NetworkStream &stream)
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

   const uint8 *NetworkStreamReader::data() const
   {
      return stream_.buffer_;
   }

   namespace {
      template <typename T>
      inline bool can_read(const NetworkStreamReader &reader, const T &value)
      {
         return (reader.at_ - reader.data()) + sizeof(T) <= reader.stream_.length_;
      }

      inline bool can_read_bytes(const NetworkStreamReader &reader, const uint64 length)
      {
         return (reader.at_ - reader.data()) + length <= reader.stream_.length_;
      }
   } // !anon

   bool NetworkStreamReader::serialize(float &value)
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

   bool NetworkStreamReader::serialize(double &value)
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

   bool NetworkStreamReader::serialize(uint8 &value)
   {
      if (!can_read(*this, value)) { return false; }
      value = at_[0];
      at_ += sizeof(value);
      return true;
   }

   bool NetworkStreamReader::serialize(int8 &value)
   {
      if (!can_read(*this, value)) { return false; }
      value = at_[0];
      at_ += sizeof(value);
      return true;
   }

   bool NetworkStreamReader::serialize(uint16 &value)
   {
      if (!can_read(*this, value)) { return false; }
      value  = ((uint16)at_[0]) << 8;
      value |= ((uint16)at_[1]);
      at_ += sizeof(value);
      return true;
   }

   bool NetworkStreamReader::serialize(int16 &value)
   {
      if (!can_read(*this, value)) { return false; }
      value  = ((int16)at_[0]) << 8;
      value |= ((int16)at_[1]);
      at_ += sizeof(value);
      return true;
   }

   bool NetworkStreamReader::serialize(uint32 &value)
   {
      if (!can_read(*this, value)) { return false; }
      value  = ((uint32)at_[0]) << 24;
      value |= ((uint32)at_[1]) << 16;
      value |= ((uint32)at_[2]) << 8;
      value |= ((uint32)at_[3]);
      at_ += sizeof(value);
      return true;
   }

   bool NetworkStreamReader::serialize(int32 &value)
   {
      if (!can_read(*this, value)) { return false; }
      value  = ((int32)at_[0]) << 24;
      value |= ((int32)at_[1]) << 16;
      value |= ((int32)at_[2]) << 8;
      value |= ((int32)at_[3]);
      at_ += sizeof(value);
      return true;
   }

   bool NetworkStreamReader::serialize(uint64 &value)
   {
      if (!can_read(*this, value)) { return false; }
      value  = ((uint64)at_[0]) << 56;
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

   bool NetworkStreamReader::serialize(int64 &value)
   {
      if (!can_read(*this, value)) { return false; }
      value  = ((int64)at_[0]) << 56;
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

   bool NetworkStreamReader::serialize(const uint64 length, uint8 *values)
   {
      if (!can_read_bytes(*this, length)) { return false; }
      memcpy(values, at_, length);
      at_ += length;
      return true;
   }

   bool NetworkStreamReader::serialize(const uint64 length, char *values)
   {
      if (!can_read_bytes(*this, length)) { return false; }
      memcpy(values, at_, length);
      at_ += length;
      return true;
   }
} // !bravo
