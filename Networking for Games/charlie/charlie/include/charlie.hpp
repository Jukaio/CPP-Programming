// charlie.hh

#ifndef CHARLIE_HH_INCLUDED
#define CHARLIE_HH_INCLUDED

#include <cassert>
#include <vector>
#include <array>
#include <queue>

namespace charlie {
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

   template <typename T, std::size_t Size>
   using StaticArray = std::array<T, Size>;

   template <typename T>
   using Queue = std::queue<T>;

   struct Color {
      static const Color Black;
      static const Color White;
      static const Color Red;
      static const Color Green;
      static const Color Blue;
      static const Color Yellow;
      static const Color Magenta;
      static const Color Aqua;
      static const Color Lime;

      Color();
      Color(const float r, const float g, const float b, const float a = 1.0f);

      float r_;
      float g_;
      float b_;
      float a_;
   };

   struct Point {
      Point();
      Point(const int32 x, const int32 y);

      Point operator+(const Point &rhs) const;
      Point operator-(const Point &rhs) const;

      bool operator==(const Point &rhs) const;
      bool operator!=(const Point &rhs) const;

      int32 x_;
      int32 y_;
   };

   struct Rectangle {
      Rectangle();
      Rectangle(const int32 x,
                const int32 y,
                const int32 width,
                const int32 height);

      bool is_inside(const Point &point) const;

      int32 x_;
      int32 y_;
      int32 width_;
      int32 height_;
   };

   struct Random {
      Random(const uint64 seed = 1);

      uint64 operator()();

      uint64 state_;
   };

   namespace math {
      template <typename T>
      inline T abs(const T &v)
      {
         return v < 0 ? -v : v;
      }

      template <typename T>
      inline T sign(const T &v)
      {
         return v < 0 ? T(1) : T(-1);
      }

      template <typename T>
      inline T min(const T &a, const T &b)
      {
         return a < b ? a : b;
      }

      template <typename T>
      inline T max(const T &a, const T &b)
      {
         return a < b ? a : b;
      }

      template <typename T>
      inline T clamp(T &v, const T &a, const T &b)
      {
         return v < a ? a : v > b ? b : v;
      }
   } // !math

   struct Vector2 {
      static const Vector2 Zero;
      static const Vector2 One;

      static float distance(const Vector2 &lhs, const Vector2 &rhs);
      static Vector2 lerp(const Vector2 &start, const Vector2 &end, const float t);
      static Vector2 min(const Vector2 &lhs, const Vector2 &rhs);
      static Vector2 max(const Vector2 &lhs, const Vector2 &rhs);
      static Vector2 unit(const Vector2 &rhs);

      Vector2();
      Vector2(const float x, const float y);
      explicit Vector2(const Point &point);
      explicit Vector2(const int32 x, const int32 y);

      Vector2 &operator-=(const Vector2 &rhs);
      Vector2 &operator+=(const Vector2 &rhs);
      Vector2 operator-(const Vector2 &rhs) const;
      Vector2 operator+(const Vector2 &rhs) const;
      Vector2 operator*(const Vector2 &rhs) const;
      Vector2 operator*(const float rhs) const;
      Vector2 operator/(const float rhs) const;

      float dot(const Vector2 &rhs) const;
      float length() const;
      void normalize();

      float x_;
      float y_;
   };

   struct Time {
      static void sleep(const Time &slice);
      static Time deltatime();
      static Time now();

      Time();
      Time(const int64 ticks);
      Time(const double seconds);

      Time &operator+=(const Time &rhs);
      Time &operator-=(const Time &rhs);
      Time operator/(const int64 rhs) const;
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

   struct FileContent {
      FileContent();
      ~FileContent();

      bool load(const char *filename);
      void release();

      uint64 size_;
      uint8 *data_;
   };

   struct Mouse {
      enum class Button {
         Left,
         Right,
         Middle,
         Count,
      };

      Mouse();

      Point position() const;
      bool down(const Button index) const;
      bool pressed(const Button index) const;
      bool released(const Button index) const;

      Point position_;
      struct {
         bool current_;
         bool previous_;
      } buttons_[int(Button::Count)];
   };

   struct Keyboard {
      enum class Key {
         None,
         Back,
         Tab,
         Clear,
         Enter,
         Shift,
         Control,
         Alt,
         Pause,
         CapsLock,
         Escape,
         Space,
         PageUp,
         PageDown,
         End,
         Home,
         Left,
         Up,
         Right,
         Down,
         PrintScreen,
         Insert,
         Del,
         Zero,  // 0
         One,   // 1
         Two,   // 2
         Three, // 3
         Four,  // 4
         Five,  // 5
         Six,   // 6
         Seven, // 7
         Eight, // 8
         Nine,  // 9
         A,
         B,
         C,
         D,
         E,
         F,
         G,
         H,
         I,
         J,
         K,
         L,
         M,
         N,
         O,
         P,
         Q,
         R,
         S,
         T,
         U,
         V,
         W,
         X,
         Y,
         Z,
         Keypad0,
         Keypad1,
         Keypad2,
         Keypad3,
         Keypad4,
         Keypad5,
         Keypad6,
         Keypad7,
         Keypad8,
         Keypad9,
         KeypadMultiply,
         KeypadAdd,
         KeypadSeparator,
         KeypadSubtract,
         KeypadDecimal,
         KeypadDivide,
         F1,
         F2,
         F3,
         F4,
         F5,
         F6,
         F7,
         F8,
         F9,
         F10,
         F11,
         F12,
         NumLock,
         ScrollLock,
         LeftShift,
         RightShift,
         LeftControl,
         RightControl,
         LeftAlt,
         RightAlt,
         Oem1,
         OemPlus,
         OemComma,
         OemMinus,
         OemPeriod,
         Oem2,
         Oem3,
         Oem4,
         Oem5,
         Oem6,
         Oem7,
         Oem8,
         Oem102,
         Count
      };

      Keyboard();

      bool down(const Key index) const;
      bool pressed(const Key index) const;
      bool released(const Key index) const;

      struct {
         bool current_;
         bool previous_;
      } keys_[int(Key::Count)];
   };

   struct Settings {
      Settings(const char *title,
               const int32 width,
               const int32 height);
      Settings(const char *title,
               const int32 width,
               const int32 height,
               const Point &position);

      enum class Style {
         Default,
         Borderless,
      };
      enum class Placement {
         Default,
         Position,
      };

      void set_style(const Style style);
      void set_placement(const Placement placement,
                         const Point &position);

      const char *title_;
      int32 width_;
      int32 height_;
      Style style_;
      Placement placement_;
      Point position_;
   };

   struct Window {
      Window();

      bool open(const Settings &settings);
      void close();
      void resize(const int32 width, const int32 height);

      bool update();
      void present();

      uint64 window_;
      uint64 device_;
      uint64 context_;
      int32 width_;
      int32 height_;
      Mouse mouse_;
      Keyboard keyboard_;
   };

   struct Texture {
      enum class Filter {
         Nearest,
         Linear,
      };

      Texture();

      bool is_valid() const;
      bool create(const char *filename, const Filter filter = Filter::Nearest);
      bool create(const int32 width, const int32 height, const void *data, const Filter filter = Filter::Nearest);
      void destroy();

      uint32 id_;
      int32 width_;
      int32 height_;
   };

   struct Transform {
      Transform();
      explicit Transform(const Vector2 &position);

      void set_origin(const Vector2 &origin);
      void set_position(const Vector2 &position);
      void set_scale(const Vector2 &scale);
      void set_rotation(const float rotation);

      Vector2 origin_;
      Vector2 position_;
      Vector2 scale_;
      float rotation_;
   };

   struct Material {
      enum class Blend {
         None,
         Alpha,
      };

      Material();

      void set_blend_mode(const Blend mode);
      void set_color(const Color &color);
      void set_texture(const Texture &texture);

      Blend blend_;
      Color color_;
      const Texture *texture_;
   };

   struct Sprite {
      Sprite();
      Sprite(const Vector2 &size,
             const Rectangle &source);

      void set_size(const Vector2 &size);
      void set_source(const Rectangle &source);

      Vector2 size_;
      Rectangle source_;
   };

   struct Renderer {
      Renderer();

      bool initialize(const Window &window);
      void shutdown();

      void clear(const Color &color);
      void projection(const int32 width, const int32 height);
      void render_text(const Point &point, const Color &color, const int32 scale, const char *text);
      void render_text_va(const Point &point, const Color &color, const int32 scale, const char *format, ...);
      void render_line(const Point &start, const Point &end, const uint32 color);
      void render_rectangle(const Rectangle &rectangle, const Color &color);
      void render_rectangle_fill(const Rectangle &rectangle, const Color &color);
      void render_sprite(const Sprite &sprite, const Material &material, const Transform &transform);
   };
} // !charlie

#endif // !CHARLIE_HH_INCLUDED
