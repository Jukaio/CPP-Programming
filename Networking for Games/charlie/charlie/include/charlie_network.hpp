// charlie_network.hpp

#ifndef CHARLIE_NETWORK_HPP_INCLUDED
#define CHARLIE_NETWORK_HPP_INCLUDED

#include <charlie.hpp>

namespace charlie {
   namespace network {
      struct Error {
         static int32 get_last();
         static bool  is_critical(const int32 error);
      };

      struct IPAddress {
         static constexpr uint32 ANY_HOST = 0;
         static bool local_addresses(DynamicArray<IPAddress> &addresses);
         static bool dns_lookup(const char *dns, DynamicArray<IPAddress> &addresses);

         IPAddress();
         IPAddress(const uint32 host,
                   const uint16 port);
         IPAddress(const uint8 a,
                   const uint8 b,
                   const uint8 c,
                   const uint8 d,
                   const uint16 port);

         bool operator==(const IPAddress &rhs) const;
         bool operator!=(const IPAddress &rhs) const;

         const char *as_string() const;

         uint32 host_;
         uint16 port_;
      };

      struct UDPSocket {
         static bool get_address(const UDPSocket &socket, IPAddress &address);

         UDPSocket();

         bool is_valid() const;
         bool open();
         bool open(const IPAddress &address);
         void close();

         bool send(const IPAddress &address, const uint8 *data, const int32 length);
         bool receive(IPAddress &address, uint8 *data, int32 &length);

         uint64 id_;
      };

      struct NetworkStream {
         NetworkStream();

         void reset();

         int32 length_;
         uint8 buffer_[1024];
      };

      struct NetworkStreamWriter {
         NetworkStreamWriter(NetworkStream &stream);

         int32 length() const;
         uint8 *data() const;

         bool serialize(const float &value);
         bool serialize(const double &value);
         bool serialize(const uint8 &value);
         bool serialize(const int8 &value);
         bool serialize(const uint16 &value);
         bool serialize(const int16 &value);
         bool serialize(const uint32 &value);
         bool serialize(const int32 &value);
         bool serialize(const uint64 &value);
         bool serialize(const int64 &value);
         bool serialize(const uint64 length, const uint8 *values);
         bool serialize(const uint64 length, const char *values);

         NetworkStream &stream_;
         uint8 *at_;
      };

      struct NetworkStreamReader {
         NetworkStreamReader(const NetworkStream &stream);

         uint8 peek() const;
         int32 length() const;
         int32 position() const;
         const uint8 *data() const;

         bool serialize(float &value);
         bool serialize(double &value);
         bool serialize(uint8 &value);
         bool serialize(int8 &value);
         bool serialize(uint16 &value);
         bool serialize(int16 &value);
         bool serialize(uint32 &value);
         bool serialize(int32 &value);
         bool serialize(uint64 &value);
         bool serialize(int64 &value);
         bool serialize(const uint64 length, uint8 *values);
         bool serialize(const uint64 length, char *values);

         const NetworkStream &stream_;
         const uint8 *at_;
      };

      struct Connection;
      struct IConnectionListener {
         virtual ~IConnectionListener() = default;
         virtual void on_acknowledge(Connection *connection, const uint16 sequence) = 0;
         virtual void on_receive(Connection *connection, NetworkStreamReader &reader) = 0;
         virtual void on_send(Connection *connection, const uint16 sequence, NetworkStreamWriter &writer) = 0;
      };

      struct Connection {
         enum class State {
            Invalid,
            Disconnected,
            SendingRequest,
            SendingResponse,
            SendingChallenge,
            Connected,
            Rejected,
            Timedout,
            Disconnecting,
         };

         Connection();

         bool is_valid() const;
         bool is_endpoint(const IPAddress &address) const;
         bool is_connected() const;
         bool is_disconnected() const;
         bool is_requesting() const;
         bool is_responding() const;
         bool is_challenging() const;
         bool is_disconnecting() const;
         bool has_timedout(const Time &time) const;
         Time latency() const;
         Time round_trip_time() const;

         void connect(const IPAddress &address);
         void disconnect();

         virtual void on_rejected(const uint8 reason);
         virtual void on_timedout();
         virtual void on_established();
         virtual void on_disconnected();

         void invalidate();
         void set_state(const State state);
         void set_address(const IPAddress &address);
         void set_key(const uint64 key);
         void set_challenge(const uint64 challenge);
         void set_sent_time(const Time &time);
         void set_received_time(const Time &time);
         void set_connected_time(const Time &time);
         void set_listener(IConnectionListener *listener);
         void receive(NetworkStreamReader &reader);
         void send();

         State state_;
         IPAddress address_;
         uint64 key_;
         uint64 challenge_;
         uint16 sequence_;
         uint16 acknowledge_;
         uint32 acknowledge_bits_;
         uint8 rejection_reason_;
         int32 disconnect_counter_;
         Time last_sent_time_;
         Time last_received_time_;
         Time connection_established_time_;
         Time round_trip_time_;
         Time round_trip_buffer_[64];
         IConnectionListener *listener_;
      };

      struct ConnectionPool {
         ConnectionPool(const int32 capacity = 32);
         ~ConnectionPool();

         void resize(const int32 capacity);

         Connection *create();
         void release(Connection *connection);

         int32 capacity_;
         int32 connection_count_;
         Connection *connections_;
      };

      struct IServiceListener {
         virtual ~IServiceListener() = default;
         virtual void on_timeout(Connection *connection) = 0;
         virtual void on_connect(Connection *connection) = 0;
         virtual void on_disconnect(Connection *connection) = 0;
      };

      struct Service {
         enum class Event {
            Timeout,
            Connect,
            Disconnect,
            Receive,
            Send,
         };

         Service();
         ~Service();

         bool initialize(const IPAddress &address);
         void shutdown();
         void update();

         void set_send_rate(const Time &rate);
         void set_allow_connections(const bool allow_connections);
         void set_connection_limit(const int32 connection_limit);

         void add_service_listener(IServiceListener *listener);
         void remove_service_listener(IServiceListener *listener);
         void notify_service_listeners(const Event event, Connection *connection);

         void add_pending_connection(Connection *connection);
         void remove_pending_connection(Connection *connection);
         Connection *find_pending_connection(const IPAddress &address) const;

         void add_established_connection(Connection *connection);
         void remove_established_connection(Connection *connection);
         Connection *find_established_connection(const IPAddress &address) const;

         void handle_connection_request(const IPAddress &address, NetworkStreamReader &reader);
         void handle_connection_challenge(const IPAddress &address, NetworkStreamReader &reader);
         void handle_connection_response(const IPAddress &address, NetworkStreamReader &reader);
         void handle_connection_rejected(const IPAddress &address, NetworkStreamReader &reader);
         void handle_connection_payload(const IPAddress &address, NetworkStreamReader &reader);
         void handle_connection_disconnect(const IPAddress &address, NetworkStreamReader &reader);

         void send_connection_request(Connection *connection);
         void send_connection_response(Connection *connection);
         void send_connection_challenge(Connection *connection);
         void send_connection_rejected(Connection *connection, const uint8 reason);
         void send_connection_disconnect(Connection *connection);
         void send_stream(Connection *connection, const NetworkStream &stream);
         void send_stream(const IPAddress &address, const NetworkStream &stream);

         void perform_periodic_timeout_check(const Time &time);

         UDPSocket socket_;
         Random random_;
         bool initialized_;
         Time send_rate_;
         Time last_timeout_check_;
         bool allow_connections_;
         int32 connection_limit_;
         ConnectionPool connection_pool_;
         DynamicArray<Connection *> pending_connections_;
         DynamicArray<Connection *> established_connections_;
         DynamicArray<IServiceListener *> connection_listeners_;
      };
   } // !network
} // !charlie

#endif // !CHARLIE_NETWORK_HPP_INCLUDED
