// network.hpp

#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED

#include <bravo.hpp>

namespace bravo {
   struct Network {
      static int32 get_error();
      static bool  is_critical(const int32 error);

      Network();
      ~Network();
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

      int32 length_;
      uint8 buffer_[2048];
   };

   struct NetworkStreamWriter {
      NetworkStreamWriter(NetworkStream &stream);

      int32 length() const;
      uint8 *data() const;

      bool serialize(const float  &value);
      bool serialize(const double &value);
      bool serialize(const uint8  &value);
      bool serialize(const int8   &value);
      bool serialize(const uint16 &value);
      bool serialize(const int16  &value);
      bool serialize(const uint32 &value);
      bool serialize(const int32  &value);
      bool serialize(const uint64 &value);
      bool serialize(const int64  &value);
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

      bool serialize(float  &value);
      bool serialize(double &value);
      bool serialize(uint8  &value);
      bool serialize(int8   &value);
      bool serialize(uint16 &value);
      bool serialize(int16  &value);
      bool serialize(uint32 &value);
      bool serialize(int32  &value);
      bool serialize(uint64 &value);
      bool serialize(int64  &value);
      bool serialize(const uint64 length, uint8 *values);
      bool serialize(const uint64 length, char *values);

      const NetworkStream &stream_;
      const uint8 *at_;
   };
} // !bravo

#endif // !NETWORK_HPP_INCLUDED
