// alfa.cc

#include "alfa.hpp"

#include <cstdio>
#include <WinSock2.h>
#include <Windows.h>

namespace alfa {
   //static 
   void Time::sleep(const Time &slice)
   {
      Sleep((DWORD)(slice.as_ticks() / 1000));
   }

   Time Time::deltatime()
   {
      static Time previous;
      Time current = Time::now();
      Time delta = current - previous;
      previous = current;
      return delta;
   }

   Time Time::now()
   {
      static LARGE_INTEGER start = {};
      static LARGE_INTEGER frequecy = {};
      if (!start.QuadPart) {
         QueryPerformanceCounter(&start);
         QueryPerformanceFrequency(&frequecy);
         frequecy.QuadPart /= 1000000;
      }

      LARGE_INTEGER current = {};
      QueryPerformanceCounter(&current);

      return Time((current.QuadPart - start.QuadPart) / frequecy.QuadPart);
   }

   Time::Time()
      : ticks_(0)
   {
   }

   Time::Time(const int64 ticks)
      : ticks_(ticks)
   {
   }

   Time::Time(const double seconds)
      : ticks_(static_cast<int64>(seconds * 1000000.0))
   {
   }

   Time &Time::operator+=(const Time &rhs)
   {
      ticks_ += rhs.ticks_;
      return *this;
   }

   Time &Time::operator-=(const Time &rhs)
   {
      ticks_ -= rhs.ticks_;
      return *this;
   }

   Time Time::operator+(const Time &rhs) const
   {
      return Time(ticks_ + rhs.ticks_);
   }

   Time Time::operator-(const Time &rhs) const
   {
      return Time(ticks_ - rhs.ticks_);
   }

   bool Time::operator==(const Time &rhs) const
   {
      return ticks_ == rhs.ticks_;
   }

   bool Time::operator!=(const Time &rhs) const
   {
      return ticks_ != rhs.ticks_;
   }

   bool Time::operator< (const Time &rhs) const
   {
      return ticks_ < rhs.ticks_;
   }

   bool Time::operator<=(const Time &rhs) const
   {
      return ticks_ <= rhs.ticks_;
   }

   bool Time::operator> (const Time &rhs) const
   {
      return ticks_ > rhs.ticks_;
   }

   bool Time::operator>=(const Time &rhs) const
   {
      return ticks_ >= rhs.ticks_;
   }

   int64 Time::as_ticks() const
   {
      return ticks_;
   }

   float Time::as_seconds() const
   {
      return (float)(ticks_ * 0.000001);
   }

   float Time::as_milliseconds() const
   {
      return (float)(ticks_ * 0.001);
   }

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

   UDPSocket::~UDPSocket()
   {
        closesocket(id_);
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

   ByteStreamWriter::ByteStreamWriter(uint64 size, uint8 *base)
      : size_(size)
      , base_(base)
      , at_(base)
   {
   }

   int32 ByteStreamWriter::length() const
   {
      return static_cast<int32>(at_ - base_);
   }

   uint8 *ByteStreamWriter::data() const
   {
      return base_;
   }

   ByteStreamReader::ByteStreamReader(const uint64 size, const uint8 *base)
      : size_(size)
      , base_(base)
      , at_(base)
   {
   }

   int32 ByteStreamReader::position() const
   {
      return static_cast<int32>(at_ - base_);
   }

   const uint8 *ByteStreamReader::data() const
   {
      return base_;
   }
} // !alfa
