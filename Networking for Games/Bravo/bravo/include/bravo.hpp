// bravo.hpp

#ifndef BRAVO_HPP_INCLUDED
#define BRAVO_HPP_INCLUDED

#include <cassert>
#include <vector>
#include <queue>

namespace bravo {
   typedef unsigned long long uint64;
   typedef   signed long long int64;
   typedef unsigned int       uint32;
   typedef   signed int       int32;
   typedef unsigned short     uint16;
   typedef   signed short     int16;
   typedef unsigned char      uint8;
   typedef   signed char      int8;

   template <typename T>
   using DynamicArray = std::vector<T>;

   template <typename T>
   using Queue = std::queue<T>;

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
      Time operator/(const int64 rhs) const;
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
}

#endif // !BRAVO_HPP_INCLUDED
