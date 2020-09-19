// bravo.cc

#include "bravo.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace bravo {
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

   Time Time::operator/(const int64 rhs) const
   {
      return Time(ticks_ / rhs);
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
} // !bravo
