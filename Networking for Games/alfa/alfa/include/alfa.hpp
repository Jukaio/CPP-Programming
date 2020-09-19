// alfa.hpp

#ifndef ALFA_HPP_INCLUDED
#define ALFA_HPP_INCLUDED

#include <cassert>

namespace alfa {
   typedef unsigned long long uint64;
   typedef   signed long long int64;
   typedef unsigned int       uint32;
   typedef   signed int       int32;
   typedef unsigned short     uint16;
   typedef   signed short     int16;
   typedef unsigned char      uint8;
   typedef   signed char      int8;

   struct Time {
      static void sleep(const Time &slice);
      static Time deltatime();
      static Time now();

      Time();
      Time(const int64 ticks);
      Time(const double seconds);

      Time &operator+=(const Time &rhs);
      Time &operator-=(const Time &rhs);
      Time operator+(const Time &rhs) const;
      Time operator-(const Time &rhs) const;
      bool operator==(const Time &rhs) const;
      bool operator!=(const Time &rhs) const;
      bool operator< (const Time &rhs) const;
      bool operator<=(const Time &rhs) const;
      bool operator> (const Time &rhs) const;
      bool operator>=(const Time &rhs) const;

      int64 as_ticks() const;
      float as_seconds() const;
      float as_milliseconds() const;

      int64 ticks_;
   };

   struct Network {
      static int32 get_error();
      static bool  is_critical(const int32 error);

      Network();
      ~Network();
   };
   struct IPAddress {
      static constexpr uint32 ANY_HOST = 0;

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
      ~UDPSocket();

      bool is_valid() const;
      bool open();
      bool open(const IPAddress &address);
      void close();

      bool send(const IPAddress &address, const uint8 *data, const int32 length);
      bool receive(IPAddress &address, uint8 *data, int32 &length);

      uint64 id_;
   };
   struct ByteStreamWriter {
      ByteStreamWriter(const uint64 size, uint8 *base);

      int32 length() const;
      uint8 *data() const;

      template <typename T>
      bool serialize(const T &value)
      {
         if ((at_ - base_) + sizeof(T) > size_) {
            return false;
         }

         *((T *)at_) = value;
         at_ += sizeof(T);

         return true;
      }

      const uint64 size_;
      uint8 *base_;
      uint8 *at_;
   };
   struct ByteStreamReader {
      ByteStreamReader(const uint64 size, const uint8 *base);

      int32 position() const;
      const uint8 *data() const;

      template <typename T>
      bool peek(T &value)
      {
         if ((at_ - base_) + sizeof(T) > size_) {
            return false;
         }

         value = *((T *)at_);

         return true;
      }

      template <typename T>
      bool serialize(T &value)
      {
         if ((at_ - base_) + sizeof(T) > size_) {
            return false;
         }

         value = *((T *)at_);
         at_ += sizeof(T);

         return true;
      }

      const uint64 size_;
      const uint8 *base_;
      const uint8 *at_;
   };

   enum MessageType
   {
       MESSAGE_TYPE_HELLO_WORLD = 1,
    };

    struct MessageHelloWorld
    { 
        MessageHelloWorld()
            : m_type(MESSAGE_TYPE_HELLO_WORLD)
            , m_sequence(0)
            , m_ack(0)
        {

        }

       bool read(ByteStreamReader& reader)
       {
            return serialize(reader);
       }
       bool write(ByteStreamWriter& writer)
       {
           return serialize(writer);
       }

       template<typename Stream>
       bool serialize(Stream &stream)
       {
           bool result = true;
           result &= stream.serialize(m_type);
           result &= stream.serialize(m_sequence);
           result &= stream.serialize(m_ack);
           return result;
       }
       uint8 m_type;
       uint32 m_sequence;
       uint32 m_ack;
   };
} // !alfa

#endif // !ALFA_HPP_INCLUDED
