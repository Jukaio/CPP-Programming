// charlie_charlie.cc

#include <charlie.hpp>

#define NOMINMAX
#include <Windows.h>
#include <windowsx.h>
#include <ShellScalingApi.h> // SetProcessDpiAwareness
#include <gl/GL.h>
#include <cmath>

struct input_t {
   charlie::Mouse mouse_;
   charlie::Keyboard keyboard_;
};

static HWND    g_window;
static HDC     g_device;
static HGLRC   g_context;
static input_t g_input;

static inline void
set_pixel(int width, int height, unsigned *dst, int x, int y, unsigned src)
{
   if (x < 0 || x >= width) return;
   if (y < 0 || y >= height) return;
   dst[y * width + x] = src;
}

static void
blit_glyphs(int width, int height, unsigned *bitmap)
{
   // Source:
   // - https://github.com/dhepper/font8x8
   //
   // license: 
   // - Public Domain
   //
   const unsigned char font8x8_basic[][8] =
   {
      { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},   // U+0021 (!)
      { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0022 (")
      { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},   // U+0023 (#)
      { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},   // U+0024 ($)
      { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},   // U+0025 (%)
      { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},   // U+0026 (&)
      { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0027 (')
      { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},   // U+0028 (()
      { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},   // U+0029 ())
      { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},   // U+002A (*)
      { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},   // U+002B (+)
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+002C (,)
      { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},   // U+002D (-)
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+002E (.)
      { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},   // U+002F (/)
      { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},   // U+0030 (0)
      { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   // U+0031 (1)
      { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   // U+0032 (2)
      { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   // U+0033 (3)
      { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   // U+0034 (4)
      { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   // U+0035 (5)
      { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   // U+0036 (6)
      { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   // U+0037 (7)
      { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+0038 (8)
      { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},   // U+0039 (9)
      { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+003A (:)
      { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+003B (//)
      { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},   // U+003C (<)
      { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},   // U+003D (=)
      { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},   // U+003E (>)
      { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},   // U+003F (?)
      { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},   // U+0040 (@)
      { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},   // U+0041 (A)
      { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},   // U+0042 (B)
      { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},   // U+0043 (C)
      { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},   // U+0044 (D)
      { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},   // U+0045 (E)
      { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},   // U+0046 (F)
      { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},   // U+0047 (G)
      { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},   // U+0048 (H)
      { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0049 (I)
      { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},   // U+004A (J)
      { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},   // U+004B (K)
      { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},   // U+004C (L)
      { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},   // U+004D (M)
      { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},   // U+004E (N)
      { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},   // U+004F (O)
      { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},   // U+0050 (P)
      { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},   // U+0051 (Q)
      { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},   // U+0052 (R)
      { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},   // U+0053 (S)
      { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0054 (T)
      { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},   // U+0055 (U)
      { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0056 (V)
      { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},   // U+0057 (W)
      { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},   // U+0058 (X)
      { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   // U+0059 (Y)
      { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},   // U+005A (Z)
      { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},   // U+005B ([)
      { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},   // U+005C (\)
      { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},   // U+005D (])
      { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},   // U+005E (^)
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},   // U+005F (_)
      { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0060 (`)
      { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},   // U+0061 (a)
      { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},   // U+0062 (b)
      { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},   // U+0063 (c)
      { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00},   // U+0064 (d)
      { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00},   // U+0065 (e)
      { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00},   // U+0066 (f)
      { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0067 (g)
      { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},   // U+0068 (h)
      { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0069 (i)
      { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},   // U+006A (j)
      { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},   // U+006B (k)
      { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+006C (l)
      { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},   // U+006D (m)
      { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},   // U+006E (n)
      { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+006F (o)
      { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},   // U+0070 (p)
      { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},   // U+0071 (q)
      { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},   // U+0072 (r)
      { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},   // U+0073 (s)
      { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+0074 (t)
      { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},   // U+0075 (u)
      { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0076 (v)
      { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},   // U+0077 (w)
      { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},   // U+0078 (x)
      { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0079 (y)
      { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},   // U+007A (z)
      { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},   // U+007B ({)
      { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},   // U+007C (|)
      { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},   // U+007D (})
      { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+007E (~)
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+007F
   };

   const int glyph_atlas_columns = 16;
   const int glyph_atlas_rows = 8;
   const int glyph_count = (sizeof(font8x8_basic) / sizeof(font8x8_basic[0]));
   for (int glyph = 0; glyph < glyph_count; glyph++)
   {
      const int glyph_x = glyph % glyph_atlas_columns * 8;
      const int glyph_y = glyph / glyph_atlas_columns * 8;

      for (int y = 0; y < 8; y++)
      {
         for (int x = 0; x < 8; x++)
         {
            if (font8x8_basic[glyph][y] & 1 << x)
               set_pixel(width, height, bitmap, glyph_x + x, glyph_y + y, 0xffffffff);
            else
               set_pixel(width, height, bitmap, glyph_x + x, glyph_y + y, 0x00000000);
         }
      }
   }

   set_pixel(width, height, bitmap, width - 1, height - 1, 0xffffffff);
}

static inline charlie::Keyboard::Key
win32_convert_virtual(WPARAM wParam)
{
   switch (wParam) {
      case VK_BACK: return charlie::Keyboard::Key::Back;
      case VK_TAB: return charlie::Keyboard::Key::Tab;
      case VK_CLEAR: return charlie::Keyboard::Key::Clear;
      case VK_RETURN: return charlie::Keyboard::Key::Enter;
      case VK_SHIFT: return charlie::Keyboard::Key::Shift;
      case VK_CONTROL: return charlie::Keyboard::Key::Control;
      case VK_MENU: return charlie::Keyboard::Key::Alt;
      case VK_PAUSE: return charlie::Keyboard::Key::Pause;
      case VK_CAPITAL: return charlie::Keyboard::Key::CapsLock;
      case VK_ESCAPE: return charlie::Keyboard::Key::Escape;
      case VK_SPACE: return charlie::Keyboard::Key::Space;
      case VK_PRIOR: return charlie::Keyboard::Key::PageUp;
      case VK_NEXT: return charlie::Keyboard::Key::PageDown;
      case VK_END: return charlie::Keyboard::Key::End;
      case VK_HOME: return charlie::Keyboard::Key::Home;
      case VK_LEFT: return charlie::Keyboard::Key::Left;
      case VK_UP: return charlie::Keyboard::Key::Up;
      case VK_RIGHT: return charlie::Keyboard::Key::Right;
      case VK_DOWN: return charlie::Keyboard::Key::Down;
      case VK_PRINT: return charlie::Keyboard::Key::PrintScreen;
      case VK_INSERT: return charlie::Keyboard::Key::Insert;
      case VK_DELETE: return charlie::Keyboard::Key::Del;
      case '0': return charlie::Keyboard::Key::Zero;
      case '1': return charlie::Keyboard::Key::One;
      case '2': return charlie::Keyboard::Key::Two;
      case '3': return charlie::Keyboard::Key::Three;
      case '4': return charlie::Keyboard::Key::Four;
      case '5': return charlie::Keyboard::Key::Five;
      case '6': return charlie::Keyboard::Key::Six;
      case '7': return charlie::Keyboard::Key::Seven;
      case '8': return charlie::Keyboard::Key::Eight;
      case '9': return charlie::Keyboard::Key::Nine;
      case 'A': return charlie::Keyboard::Key::A;
      case 'B': return charlie::Keyboard::Key::B;
      case 'C': return charlie::Keyboard::Key::C;
      case 'D': return charlie::Keyboard::Key::D;
      case 'E': return charlie::Keyboard::Key::E;
      case 'F': return charlie::Keyboard::Key::F;
      case 'G': return charlie::Keyboard::Key::G;
      case 'H': return charlie::Keyboard::Key::H;
      case 'I': return charlie::Keyboard::Key::I;
      case 'J': return charlie::Keyboard::Key::J;
      case 'K': return charlie::Keyboard::Key::K;
      case 'L': return charlie::Keyboard::Key::L;
      case 'M': return charlie::Keyboard::Key::M;
      case 'N': return charlie::Keyboard::Key::N;
      case 'O': return charlie::Keyboard::Key::O;
      case 'P': return charlie::Keyboard::Key::P;
      case 'Q': return charlie::Keyboard::Key::Q;
      case 'R': return charlie::Keyboard::Key::R;
      case 'S': return charlie::Keyboard::Key::S;
      case 'T': return charlie::Keyboard::Key::T;
      case 'U': return charlie::Keyboard::Key::U;
      case 'V': return charlie::Keyboard::Key::V;
      case 'W': return charlie::Keyboard::Key::W;
      case 'X': return charlie::Keyboard::Key::X;
      case 'Y': return charlie::Keyboard::Key::Y;
      case 'Z': return charlie::Keyboard::Key::Z;
      case VK_NUMPAD0: return charlie::Keyboard::Key::Keypad0;
      case VK_NUMPAD1: return charlie::Keyboard::Key::Keypad1;
      case VK_NUMPAD2: return charlie::Keyboard::Key::Keypad2;
      case VK_NUMPAD3: return charlie::Keyboard::Key::Keypad3;
      case VK_NUMPAD4: return charlie::Keyboard::Key::Keypad4;
      case VK_NUMPAD5: return charlie::Keyboard::Key::Keypad5;
      case VK_NUMPAD6: return charlie::Keyboard::Key::Keypad6;
      case VK_NUMPAD7: return charlie::Keyboard::Key::Keypad7;
      case VK_NUMPAD8: return charlie::Keyboard::Key::Keypad8;
      case VK_NUMPAD9: return charlie::Keyboard::Key::Keypad9;
      case VK_MULTIPLY: return charlie::Keyboard::Key::KeypadMultiply;
      case VK_ADD: return charlie::Keyboard::Key::KeypadAdd;
      case VK_SEPARATOR: return charlie::Keyboard::Key::KeypadSeparator;
      case VK_SUBTRACT: return charlie::Keyboard::Key::KeypadSubtract;
      case VK_DECIMAL: return charlie::Keyboard::Key::KeypadDecimal;
      case VK_DIVIDE: return charlie::Keyboard::Key::KeypadDivide;
      case VK_F1: return charlie::Keyboard::Key::F1;
      case VK_F2: return charlie::Keyboard::Key::F2;
      case VK_F3: return charlie::Keyboard::Key::F3;
      case VK_F4: return charlie::Keyboard::Key::F4;
      case VK_F5: return charlie::Keyboard::Key::F5;
      case VK_F6: return charlie::Keyboard::Key::F6;
      case VK_F7: return charlie::Keyboard::Key::F7;
      case VK_F8: return charlie::Keyboard::Key::F8;
      case VK_F9: return charlie::Keyboard::Key::F9;
      case VK_F10: return charlie::Keyboard::Key::F10;
      case VK_F11: return charlie::Keyboard::Key::F11;
      case VK_F12: return charlie::Keyboard::Key::F12;
      case VK_NUMLOCK: return charlie::Keyboard::Key::NumLock;
      case VK_SCROLL: return charlie::Keyboard::Key::ScrollLock;
      case VK_LSHIFT: return charlie::Keyboard::Key::LeftShift;
      case VK_RSHIFT: return charlie::Keyboard::Key::RightShift;
      case VK_LCONTROL: return charlie::Keyboard::Key::LeftControl;
      case VK_RCONTROL: return charlie::Keyboard::Key::RightControl;
      case VK_LMENU: return charlie::Keyboard::Key::LeftAlt;
      case VK_RMENU: return charlie::Keyboard::Key::RightAlt;
      case VK_OEM_1: return charlie::Keyboard::Key::Oem1;
      case VK_OEM_PLUS: return charlie::Keyboard::Key::OemPlus;
      case VK_OEM_COMMA: return charlie::Keyboard::Key::OemComma;
      case VK_OEM_MINUS: return charlie::Keyboard::Key::OemMinus;
      case VK_OEM_PERIOD: return charlie::Keyboard::Key::OemPeriod;
      case VK_OEM_2: return charlie::Keyboard::Key::Oem2;
      case VK_OEM_3: return charlie::Keyboard::Key::Oem3;
      case VK_OEM_4: return charlie::Keyboard::Key::Oem4;
      case VK_OEM_5: return charlie::Keyboard::Key::Oem5;
      case VK_OEM_6: return charlie::Keyboard::Key::Oem6;
      case VK_OEM_7: return charlie::Keyboard::Key::Oem7;
      case VK_OEM_8: return charlie::Keyboard::Key::Oem8;
      case VK_OEM_102: return charlie::Keyboard::Key::Oem102;
   }

   return charlie::Keyboard::Key::None;
}

static inline WPARAM
win32_convert_extended(WPARAM wParam, LPARAM lParam)
{
   WPARAM result = wParam;
   UINT scancode = (lParam & 0x00ff0000) >> 16;
   int  extended = (lParam & 0x01000000) != 0;

   switch (wParam) {
      case VK_SHIFT:
         result = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
         break;
      case VK_CONTROL:
         result = extended ? VK_RCONTROL : VK_LCONTROL;
         break;
      case VK_MENU:
         result = extended ? VK_RMENU : VK_LMENU;
         break;
   }

   return result;
}

static LRESULT CALLBACK
win32_main_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   LRESULT result = 0;
   switch (uMsg) {
      case WM_MOUSEMOVE:
      {
         g_input.mouse_.position_.x_ = GET_X_LPARAM(lParam);
         g_input.mouse_.position_.y_ = GET_Y_LPARAM(lParam);
      } break;

      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
      {
         g_input.mouse_.buttons_[int(charlie::Mouse::Button::Left)].current_ = uMsg == WM_LBUTTONDOWN;
      } break;
      case WM_RBUTTONDOWN:
      case WM_RBUTTONUP:
      {
         g_input.mouse_.buttons_[int(charlie::Mouse::Button::Right)].current_ = uMsg == WM_RBUTTONDOWN;
      } break;
      case WM_MBUTTONDOWN:
      case WM_MBUTTONUP:
      {
         g_input.mouse_.buttons_[int(charlie::Mouse::Button::Middle)].current_ = uMsg == WM_MBUTTONDOWN;
      } break;

      case WM_KEYDOWN:
      case WM_KEYUP:
      {
         wParam = win32_convert_extended(wParam, lParam);
         if (wParam < 256) {
            auto key = win32_convert_virtual(wParam);
            g_input.keyboard_.keys_[int(key)].current_ = uMsg == WM_KEYDOWN;
         }
      } break;

      case WM_CLOSE:
      {
         PostQuitMessage(0);
      } break;

      default:
      {
         result = DefWindowProcA(hWnd, uMsg, wParam, lParam);
      } break;
   }

   return result;
}

namespace charlie {
   // static 
   const Color Color::Black(0.0f, 0.0f, 0.0f, 1.0f);
   const Color Color::White(1.0f, 1.0f, 1.0f, 1.0f);
   const Color Color::Red(1.0f, 0.0f, 0.0f, 1.0f);
   const Color Color::Green(0.0f, 0.5f, 0.0f, 1.0f);
   const Color Color::Blue(0.0f, 0.0f, 1.0f, 1.0f);
   const Color Color::Yellow(1.0f, 1.0f, 0.0f, 1.0f);
   const Color Color::Magenta(1.0f, 0.0f, 1.0f, 1.0f);
   const Color Color::Aqua(0.0f, 1.0f, 1.0f, 1.0f);
   const Color Color::Lime(0.0f, 1.0f, 0.0f, 1.0f);

   Color::Color()
      : r_(0.0f)
      , g_(0.0f)
      , b_(0.0f)
      , a_(1.0f)
   {
   }

   Color::Color(const float r,
                const float g,
                const float b,
                const float a)
      : r_(r)
      , g_(g)
      , b_(b)
      , a_(a)
   {
   }

   Point::Point()
      : x_(0)
      , y_(0)
   {
   }

   Point::Point(const int32 x, const int32 y)
      : x_(x)
      , y_(y)
   {
   }

   Point Point::operator+(const Point &rhs) const
   {
      return Point(x_ + rhs.x_, y_ + rhs.y_);
   }

   Point Point::operator-(const Point &rhs) const
   {
      return Point(x_ - rhs.x_, y_ - rhs.y_);
   }

   bool Point::operator==(const Point &rhs) const
   {
      return x_ == rhs.x_ && y_ == rhs.y_;
   }

   bool Point::operator!=(const Point &rhs) const
   {
      return x_ != rhs.x_ || y_ != rhs.y_;
   }

   Rectangle::Rectangle()
      : x_(0)
      , y_(0)
      , width_(0)
      , height_(0)
   {
   }

   Rectangle::Rectangle(const int32 x,
                        const int32 y,
                        const int32 width,
                        const int32 height)
      : x_(x)
      , y_(y)
      , width_(width)
      , height_(height)
   {
   }

   bool Rectangle::is_inside(const Point &point) const 
   {
      return (point.x_ > x_ && point.x_ < x_ + width_) &&
             (point.y_ > y_ && point.y_ < y_ + height_);
   }

   // source: github.com/ademakov/RNG/blob/master/rng.h
   Random::Random(const uint64 seed)
      : state_(seed)
   {
   }

   uint64 Random::operator()()
   {
      state_ += 0x9e3779b97f4a7c15ull;
      uint64 result = state_;
      result = (result ^ (result >> 30)) * 0xbf58476d1ce4e5b9ull;
      result = (result ^ (result >> 27)) * 0x94d049bb133111ebull;
      return result ^ (result >> 31);
   }

   // static
   const Vector2 Vector2::Zero = { 0.0f, 0.0f };
   const Vector2 Vector2::One = { 1.0f, 1.0f };

   float Vector2::distance(const Vector2 &lhs, const Vector2 &rhs)
   {
      return (lhs - rhs).length();
   }

   Vector2 Vector2::lerp(const Vector2 &start, const Vector2 &end, const float t)
   {
      if (t <= 0.0f) {
         return start;
      }
      else if (t >= 1.0f) {
         return end;
      }

      return Vector2(start.x_ * (1.0f - t) + end.x_ * (t),
                     start.y_ * (1.0f - t) + end.y_ * (t));
   }

   Vector2 Vector2::min(const Vector2 &lhs, const Vector2 &rhs)
   {
      float x_min = lhs.x_ < rhs.x_ ? lhs.x_ : rhs.x_;
      float y_min = lhs.y_ < rhs.y_ ? lhs.y_ : rhs.y_;
      return Vector2(x_min, y_min);
   }

   Vector2 Vector2::max(const Vector2 &lhs, const Vector2 &rhs)
   {
      float x_max = lhs.x_ > rhs.x_ ? lhs.x_ : rhs.x_;
      float y_max = lhs.y_ > rhs.y_ ? lhs.y_ : rhs.y_;
      return Vector2(x_max, y_max);
   }

   Vector2 Vector2::unit(const Vector2 &rhs)
   {
      Vector2 result(rhs);
      result.normalize();
      return result;
   }

   Vector2::Vector2()
      : x_(0.0f)
      , y_(0.0f)
   {
   }

   Vector2::Vector2(const float x, const float y)
      : x_(x)
      , y_(y)
   {
   }

   Vector2::Vector2(const Point &point)
      : x_(static_cast<float>(point.x_))
      , y_(static_cast<float>(point.y_))
   {
   }

   Vector2::Vector2(const int32 x, const int32 y)
      : x_(static_cast<float>(x))
      , y_(static_cast<float>(y))
   {
   }

   Vector2 &Vector2::operator-=(const Vector2 &rhs)
   {
      x_ -= rhs.x_;
      y_ -= rhs.y_;
      return *this;
   }

   Vector2 &Vector2::operator+=(const Vector2 &rhs)
   {
      x_ += rhs.x_;
      y_ += rhs.y_;
      return *this;
   }

   Vector2 Vector2::operator-(const Vector2 &rhs) const
   {
      return Vector2(x_ - rhs.x_,
                     y_ - rhs.y_);
   }

   Vector2 Vector2::operator+(const Vector2 &rhs) const
   {
      return Vector2(x_ + rhs.x_,
                     y_ + rhs.y_);
   }

   Vector2 Vector2::operator*(const Vector2 &rhs) const
   {
      return Vector2(x_ * rhs.x_,
                     y_ * rhs.y_);
   }

   Vector2 Vector2::operator*(const float rhs) const
   {
      return Vector2(x_ * rhs,
                     y_ * rhs);
   }

   Vector2 Vector2::operator/(const float rhs) const
   {
      return Vector2(x_ / rhs,
                     y_ / rhs);
   }

   float Vector2::dot(const Vector2 &rhs) const
   {
      return x_ * x_ + y_ * y_;
   }

   float Vector2::length() const
   {
      return std::sqrtf(x_ * x_ + y_ * y_);
   }

   void Vector2::normalize()
   {
      const float len = length();
      if (len > 0.0f) {
         const float inv_len = 1.0f / len;
         x_ *= inv_len;
         y_ *= inv_len;
      }
   }

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

   Time Time::operator/(const int64 rhs) const
   {
      return Time(ticks_ / rhs);
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

   FileContent::FileContent()
      : size_(0)
      , data_(nullptr)
   {
   }

   FileContent::~FileContent()
   {
      release();
   }

   bool FileContent::load(const char *filename)
   {
      HANDLE handle = CreateFileA(filename,
                                  GENERIC_READ,
                                  FILE_SHARE_READ,
                                  NULL,
                                  OPEN_EXISTING,
                                  FILE_ATTRIBUTE_NORMAL,
                                  NULL);
      if (handle == INVALID_HANDLE_VALUE) {
         return false;
      }

      DWORD size = GetFileSize(handle, NULL);
      if (size == 0) {
         CloseHandle(handle);
         return false;
      }

      uint8 *data = (uint8 *)VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
      if (!ReadFile(handle, data, size, NULL, NULL)) {
         VirtualFree(data, 0, MEM_RELEASE);
         CloseHandle(handle);
         return false;
      }

      CloseHandle(handle);

      size_ = size;
      data_ = data;

      return true;
   }

   void FileContent::release()
   {
      if (data_) {
         VirtualFree(data_, 0, MEM_RELEASE);
      }

      size_ = 0;
      data_ = nullptr;
   }

   Mouse::Mouse()
      : buttons_{}
   {
   }

   Point Mouse::position() const
   {
      return position_;
   }

   bool Mouse::down(const Button index) const
   {
      return buttons_[int(index)].current_;
   }

   bool Mouse::pressed(const Button index) const
   {
      return buttons_[int(index)].current_ && !buttons_[int(index)].previous_;
   }

   bool Mouse::released(const Button index) const
   {
      return !buttons_[int(index)].current_ && buttons_[int(index)].previous_;
   }

   Keyboard::Keyboard()
      : keys_{}
   {
   }

   bool Keyboard::down(const Key index) const
   {
      return keys_[int(index)].current_;
   }

   bool Keyboard::pressed(const Key index) const
   {
      return keys_[int(index)].current_ && !keys_[int(index)].previous_;
   }

   bool Keyboard::released(const Key index) const
   {
      return !keys_[int(index)].current_ && keys_[int(index)].previous_;
   }

   Settings::Settings(const char *title,
                      const int32 width,
                      const int32 height)
      : title_(title)
      , width_(width)
      , height_(height)
      , style_{}
      , placement_{}
      , position_{}
   {
   }

   Settings::Settings(const char *title,
                      const int32 width,
                      const int32 height,
                      const Point &position)
      : title_(title)
      , width_(width)
      , height_(height)
      , style_{}
      , placement_(Placement::Position)
      , position_(position)
   {
   }

   void Settings::set_style(const Settings::Style style)
   {
      style_ = style;
   }

   void Settings::set_placement(const Settings::Placement placement,
                                const Point &position)
   {
      placement_ = placement;
      position_ = position;
   }

   Window::Window()
      : window_(0)
      , device_(0)
      , context_(0)
      , width_(0)
      , height_(0)
   {
   }

   bool Window::open(const Settings &settings)
   {
      Time::now();
      SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);

      WSADATA data = {};
      WSAStartup(MAKEWORD(2, 2), &data);

      WNDCLASSA wc = {};
      wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
      wc.lpfnWndProc = win32_main_proc;
      wc.hInstance = GetModuleHandle(NULL);
      wc.lpszClassName = "interpolationClassName";
      wc.hCursor = LoadCursor(NULL, IDC_ARROW);
      wc.hbrBackground = CreateSolidBrush(0x00000000);
      wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(101));
      if (!RegisterClassA(&wc)) {
         return false;
      }

      DWORD ws = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU);
      if (settings.style_ == Settings::Style::Borderless) {
         ws = WS_POPUP;
      }

      RECT wr = { 0, 0, settings.width_, settings.height_ };
      if (!AdjustWindowRect(&wr, ws, FALSE)) {
         return false;
      }

      Point wp = { CW_USEDEFAULT, CW_USEDEFAULT };
      if (settings.placement_ == Settings::Placement::Position) {
         wp = settings.position_;
      }

      HWND window = CreateWindowA(wc.lpszClassName,
                                  settings.title_,
                                  ws,
                                  wp.x_,
                                  wp.y_,
                                  wr.right - wr.left,
                                  wr.bottom - wr.top,
                                  0, 0,
                                  wc.hInstance,
                                  0);
      if (!window) {
         return false;
      }

      HDC device = GetDC(window);

      PIXELFORMATDESCRIPTOR pfd = { 0 };
      pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
      pfd.nVersion = 1;
      pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
      pfd.iPixelType = PFD_TYPE_RGBA;
      pfd.cColorBits = 32;
      pfd.iLayerType = PFD_MAIN_PLANE;

      int pixel_format_index = ChoosePixelFormat(device, &pfd);
      SetPixelFormat(device, pixel_format_index, &pfd);

      HGLRC context = wglCreateContext(device);
      if (!wglMakeCurrent(device, context)) {
         return false;
      }

      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_COLOR_ARRAY);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);

      glEnable(GL_TEXTURE_2D);
      glEnable(GL_CULL_FACE);
      glFrontFace(GL_CW);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      //opengl_init_font_texture();
      //opengl_init_white_texture();

      ShowWindow(window, SW_SHOWNORMAL);
      UpdateWindow(window);

      window_ = (uint64)window;
      device_ = (uint64)device;
      context_ = (uint64)context;
      width_ = settings.width_;
      height_ = settings.height_;

      return true;
   }

   void Window::close()
   {
      wglMakeCurrent(NULL, NULL);
      wglDeleteContext((HGLRC)context_);
      ReleaseDC((HWND)window_, (HDC)device_);
      CloseWindow((HWND)window_);
      
      window_ = 0;
      device_ = 0;
      context_ = 0;
   }

   static inline void opengl_projection(const int32 width, const int32 height)
   {
      glViewport(0, 0, width, height);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
   }

   void Window::resize(const int32 width, const int32 height)
   {
      RECT window_rect = { 0 };
      GetWindowRect((HWND)window_, &window_rect);

      int window_x = window_rect.left;
      int window_y = window_rect.top;

      window_rect.right = width;
      window_rect.bottom = height;
      DWORD style = GetWindowStyle((HWND)window_);
      AdjustWindowRect(&window_rect, style, 0);

      int window_width = window_rect.right - window_rect.left;
      int window_height = window_rect.bottom - window_rect.top;
      SetWindowPos((HWND)window_,
                   NULL,
                   window_x,
                   window_y,
                   window_width,
                   window_height,
                   SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

      opengl_projection(width, height);

      width_ = width;
      height_ = height;
   }

   bool Window::update()
   {
      for (auto &key : g_input.keyboard_.keys_) {
         key.previous_ = key.current_;
      }
      for (auto &button : g_input.mouse_.buttons_) {
         button.previous_ = button.current_;
      }

      MSG msg = {};
      while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
         if (msg.message == WM_QUIT) {
            return false;
         }

         TranslateMessage(&msg);
         DispatchMessageA(&msg);
      }

      mouse_ = g_input.mouse_;
      keyboard_ = g_input.keyboard_;

      return true;
   }

   void Window::present()
   {
      SwapBuffers((HDC)device_);
   }

   //
   static Texture g_font;

   //static void opengl_init_font_texture()
   //{
   //   unsigned bitmap[16384] = {};
   //   blit_glyphs(128, 128, bitmap);
   //
   //   GLuint id = 0;
   //   glGenTextures(1, &id);
   //   glBindTexture(GL_TEXTURE_2D, id);
   //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   //   glTexImage2D(GL_TEXTURE_2D,
   //                0,
   //                GL_RGBA,
   //                128,
   //                128,
   //                0,
   //                GL_RGBA,
   //                GL_UNSIGNED_BYTE,
   //                bitmap);
   //
   //   if (glGetError() != GL_NO_ERROR) {
   //      *(int *)0 = 0;
   //   }
   //
   //   g_font = id;
   //}
   //
   //static void opengl_init_white_texture()
   //{
   //   unsigned bitmap[] =
   //   {
   //      0xffffffff, 0xffffffff,
   //      0xffffffff, 0xffffffff,
   //   };
   //
   //   GLuint id = 0;
   //   glGenTextures(1, &id);
   //   glBindTexture(GL_TEXTURE_2D, id);
   //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   //   glTexImage2D(GL_TEXTURE_2D,
   //                0,
   //                GL_RGBA,
   //                2,
   //                2,
   //                0,
   //                GL_RGBA,
   //                GL_UNSIGNED_BYTE,
   //                bitmap);
   //
   //   if (glGetError() != GL_NO_ERROR) {
   //      *(int *)0 = 0;
   //   }
   //
   //   g_white = id;
   //}


#pragma pack(push, 1)
   struct bmp_header_t {
      uint8  magic_[2];
      uint32 ignored_[2];
      uint32 offset_;
   };

   struct dib_header_t {
      uint32 size_;
      uint32 width_;
      uint32 height_;
      uint16 planes_;
      uint16 bits_per_pixel_;
      uint32 compression_;
      uint32 image_size_;
      uint32 horizontal_resolution_;
      uint32 vertical_resolution_;
      uint32 color_count_;
      uint32 important_color_count_;
   };
#pragma pack(pop)

   Texture::Texture()
      : id_(0)
      , width_(0)
      , height_(0)
   {
   }

   bool Texture::is_valid() const
   {
      return id_ != 0;
   }

   bool Texture::create(const char *filename, const Filter filter)
   {
      if (is_valid()) {
         destroy();
      }

      FileContent content;
      if (!content.load(filename)) {
         return false;
      }

      const uint8 *at = content.data_;
      bmp_header_t *bmp = (bmp_header_t *)at;
      at += sizeof(*bmp);

      if (bmp->magic_[0] != 'B' && bmp->magic_[1] != 'M') {
         return false;
      }

      dib_header_t *dib = (dib_header_t *)at;
      at = content.data_ + bmp->offset_;

      if (dib->bits_per_pixel_ != 24 && dib->bits_per_pixel_ != 32) {
         return false;
      }

      if (dib->compression_ != 0 && dib->compression_ != 3) {
         return false;
      }

      const int32 bpp = dib->bits_per_pixel_ / 8;
      const int32 width = dib->width_;
      const int32 height = dib->height_;
      const int32 stride = width * bpp;
      const uint8 *src = at + (height - 1) * stride;

      DynamicArray<uint32> data(width * height, 0);
      for (int32 y = 0; y < height; y++) {
         uint32 *dst = data.data() + y * width;
         for (int32 x = 0; x < width; x++) {
            const int32 index = x * bpp;
            const uint32 r = src[index + 0];
            const uint32 g = src[index + 1];
            const uint32 b = src[index + 2];
            uint32 a = 0xff;

            if (bpp == 4) {
               a = src[index + 3];
            }

            uint32 rgba = 0;
            rgba |= (r << 16);
            rgba |= (g << 8);
            rgba |= (b << 0);
            rgba |= (a << 24);
            *dst++ = rgba;
         }

         src -= stride;
      }

      bool result = create(width, height, data.data(), filter);

      return result;
   }

   bool Texture::create(const int32 width, const int32 height, const void *data, const Filter filter)
   {
      const GLenum filters[] = { GL_NEAREST, GL_LINEAR };

      glGenTextures(1, &id_);
      glBindTexture(GL_TEXTURE_2D, id_);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filters[int(filter)]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filters[int(filter)]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      glTexImage2D(GL_TEXTURE_2D,
                   0,
                   GL_RGBA,
                   width,
                   height,
                   0,
                   GL_RGBA,
                   GL_UNSIGNED_BYTE,
                   data);

      if (glGetError() != GL_NO_ERROR) {
         destroy();
      }

      width_ = width;
      height_ = height;

      return is_valid();
   }

   void Texture::destroy()
   {
      if (is_valid()) {
         glDeleteTextures(1, &id_);
         id_ = 0;
      }
   }

   Transform::Transform()
      : scale_(1.0f, 1.0f)
      , rotation_(0.0f)
   {
   }

   Transform::Transform(const Vector2 &position)
      : position_(position)
      , scale_(1.0f, 1.0f)
      , rotation_(0.0f)
   {
   }

   void Transform::set_origin(const Vector2 &origin)
   {
      origin_ = origin;
   }

   void Transform::set_position(const Vector2 &position)
   {
      position_ = position;
   }

   void Transform::set_scale(const Vector2 &scale)
   {
      scale_ = scale;
   }

   void Transform::set_rotation(const float rotation)
   {
      rotation_ = rotation;
   }

   Material::Material()
      : blend_(Blend::Alpha)
      , color_(Color::White)
   {
   }

   void Material::set_blend_mode(const Blend mode)
   {
      blend_ = mode;
   }

   void Material::set_color(const Color &color)
   {
      color_ = color;
   }

   void Material::set_texture(const Texture &texture)
   {
      texture_ = &texture;
   }

   Sprite::Sprite()
   {
   }

   Sprite::Sprite(const Vector2 &size,
                  const Rectangle &source)
      : size_(size)
      , source_(source)
   {
   }

   void Sprite::set_size(const Vector2 &size)
   {
      size_ = size;
   }

   void Sprite::set_source(const Rectangle &source)
   {
      source_ = source;
   }

   struct vertex_t {
      float x_, y_;
      float u_, v_;
      float r_, g_, b_, a_;
   };

   struct Vector4 {
      float x_;
      float y_;
      float z_;
      float w_;
   };

   struct Matrix4 {
      Vector4 x_{};
      Vector4 y_{};
      Vector4 z_{};
      Vector4 w_{};
   };

   inline Matrix4 as_matrix(const Transform &transform) {
      const Vector2 origin = transform.origin_;
      const Vector2 position = transform.position_;
      const Vector2 scale = transform.scale_;
      const float rotation = transform.rotation_;

      const float angle = rotation * 3.141592654f / 180.0f;
      const float cos = cosf(angle);
      const float sin = sinf(angle);
      const float sxc = scale.x_ * cos;
      const float syc = scale.y_ * cos;
      const float sxs = scale.x_ * sin;
      const float sys = scale.y_ * sin;
      const float tx = -origin.x_ * cos - origin.y_ * sin + position.x_;
      const float ty = origin.x_ * sin - origin.y_ * cos + position.y_;

      return
      {
         {  sxc,  sys, 0.0f, 0.0f },
         { -sxs,  syc, 0.0f, 0.0f },
         { 0.0f, 0.0f, 1.0f, 0.0f },
         {   tx,   ty, 0.0f, 1.0f }
      };
   }

   inline Vector2 operator*(const Matrix4 &lhs, const Vector2 &rhs) {
      Vector2 result;
      result.x_ = lhs.x_.x_ * rhs.x_ + lhs.x_.y_ * rhs.y_ + lhs.w_.x_;
      result.y_ = lhs.y_.x_ * rhs.x_ + lhs.y_.y_ * rhs.y_ + lhs.w_.y_;
      return result;
   }

   static inline void opengl_set_color(const Color &color)
   {
      glColor4f(color.r_, color.g_, color.b_, color.a_);
   }

   static inline void opengl_set_texture(const GLuint id)
   {
      static GLuint bound = 0;
      if (bound != id) {
         bound = id;
         glBindTexture(GL_TEXTURE_2D, bound);
      }
   }

   static inline void opengl_render_mode(const GLenum mode, const uint32 count, const vertex_t *vertices)
   {
      const char *base = (const char *)vertices;
      glVertexPointer(2, GL_FLOAT, sizeof(vertex_t), base + offsetof(vertex_t, x_));
      glTexCoordPointer(2, GL_FLOAT, sizeof(vertex_t), base + offsetof(vertex_t, u_));
      glColorPointer(4, GL_FLOAT, sizeof(vertex_t), base + offsetof(vertex_t, r_));
      glDrawArrays(mode, 0, count);
   }

   static inline void opengl_render_quads(const uint32 count, const vertex_t *vertices)
   {
      opengl_render_mode(GL_QUADS, count, vertices);
   }

   Renderer::Renderer()
   {
   }

   bool Renderer::initialize(const Window &window)
   {
      opengl_projection(window.width_, window.height_);

      if (g_font.is_valid()) {
         return true;
      }

      unsigned bitmap[16384] = {};
      blit_glyphs(128, 128, bitmap);
      g_font.create(128, 128, bitmap);
      return g_font.is_valid();
   }

   void Renderer::shutdown()
   {
      if (g_font.is_valid()) {
         g_font.destroy();
      }
   }

   void Renderer::clear(const Color &color)
   {
      glClearColor(color.r_, color.g_, color.b_, color.a_);
      glClear(GL_COLOR_BUFFER_BIT);
   }

   void Renderer::projection(const int32 width, const int32 height)
   {
      opengl_projection(width, height);
   }

   void Renderer::render_text(const Point &point, const Color &color, const int32 scale, const char *text)
   {
      if (scale <= 0) {
         return;
      }

      const float uvst = 1.0f / 16.0f;
      const int character_width = 8 * scale;
      const int line_feed_height = 10 * scale;
      const int first_valid_character = (int)'!';
      const int last_valid_character = (int)'~';
      const int invalid_character = (int)'?' - first_valid_character;

      opengl_set_color(Color::White);
      opengl_set_texture(g_font.id_);

      const float r = color.r_;
      const float g = color.g_;
      const float b = color.b_;
      const float a = color.a_;

      const int vertex_capacity = 128;
      int vertex_count = 0;
      vertex_t vertices[vertex_capacity] = {};

      const int len = (int)strlen(text);
      int x = point.x_;
      int y = point.y_;
      for (size_t index = 0; index < len; index++)
      {
         int character = (int)text[index];
         if (character == (int)(' '))
         {
            x += character_width;
            continue;
         }
         else if (character == (int)('\n'))
         {
            x = point.x_;
            y += line_feed_height;
            continue;
         }

         int character_index = character - first_valid_character;
         if (character_index < 0 || character_index >= last_valid_character)
            character_index = invalid_character;

         float u = (character_index % 16) * uvst;
         float v = (character_index / 16) * uvst;
         float s = u + uvst;
         float t = v + uvst;

#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4838)
         vertices[vertex_count++] = { 1.0f * x                  , 1.0f * y                  , u, v, r, g, b, a };
         vertices[vertex_count++] = { 1.0f * x + character_width, 1.0f * y                  , s, v, r, g, b, a };
         vertices[vertex_count++] = { 1.0f * x + character_width, 1.0f * y + character_width, s, t, r, g, b, a };
         vertices[vertex_count++] = { 1.0f * x                  , 1.0f * y + character_width, u, t, r, g, b, a };
#pragma warning(pop)

         if (vertex_count == vertex_capacity) {
            opengl_render_quads(vertex_count, vertices);
            vertex_count = 0;
         }

         x += character_width;
      }

      if (vertex_count) {
         opengl_render_quads(vertex_count, vertices);
      }
   }

   void Renderer::render_text_va(const Point &point, const Color &color, const int32 scale, const char *format, ...)
   {
      char text[2048] = {};
      va_list args;
      va_start(args, format);
      size_t len = vsprintf_s(text, sizeof(text) - 1, format, args);
      va_end(args);
      render_text(point, color, scale, text);
   }

   void Renderer::render_line(const Point &start, const Point &end, const uint32 color)
   {
      const float u = 1.0f;
      const float v = 1.0f;
      const float r = ((color >> 16) & 0xff) / 255.0f;
      const float g = ((color >> 8) & 0xff) / 255.0f;
      const float b = ((color >> 0) & 0xff) / 255.0f;
      const float a = ((color >> 24) & 0xff) / 255.0f;

      const vertex_t vertices[4] =
      {
         { 1.0f * start.x_ , 1.0f * start.y_ , u, v, r, g, b, a },
         { 1.0f * end.x_   , 1.0f * end.y_   , u, v, r, g, b, a },
      };

      opengl_set_texture(g_font.id_);
      opengl_render_mode(GL_LINES, 2, vertices);
   }

   void Renderer::render_rectangle(const Rectangle &rectangle, const Color &color)
   {
      const float u = 1.0f;
      const float v = 1.0f;
      const float r = color.r_;
      const float g = color.g_;
      const float b = color.b_;
      const float a = color.a_;

      const vertex_t vertices[4] =
      {
         { 1.0f * rectangle.x_                   , 1.0f * rectangle.y_                    , u, v, r, g, b, a },
         { 1.0f * rectangle.x_ + rectangle.width_, 1.0f * rectangle.y_                    , u, v, r, g, b, a },
         { 1.0f * rectangle.x_ + rectangle.width_, 1.0f * rectangle.y_ + rectangle.height_, u, v, r, g, b, a },
         { 1.0f * rectangle.x_                   , 1.0f * rectangle.y_ + rectangle.height_, u, v, r, g, b, a },
      };

      opengl_set_texture(g_font.id_);
      opengl_render_mode(GL_LINE_LOOP, 4, vertices);
   }

   void Renderer::render_rectangle_fill(const Rectangle &rectangle, const Color &color)
   {
      const float u = 1.0f;
      const float v = 1.0f;
      const float r = color.r_;
      const float g = color.g_;
      const float b = color.b_;
      const float a = color.a_;

      const vertex_t vertices[4] =
      {
         { 1.0f * rectangle.x_                   , 1.0f * rectangle.y_                    , u, v, r, g, b, a },
         { 1.0f * rectangle.x_ + rectangle.width_, 1.0f * rectangle.y_                    , u, v, r, g, b, a },
         { 1.0f * rectangle.x_ + rectangle.width_, 1.0f * rectangle.y_ + rectangle.height_, u, v, r, g, b, a },
         { 1.0f * rectangle.x_                   , 1.0f * rectangle.y_ + rectangle.height_, u, v, r, g, b, a },
      };

      opengl_set_texture(g_font.id_);
      opengl_render_quads(4, vertices);
   }

   void Renderer::render_sprite(const Sprite &sprite, const Material &material, const Transform &transform)
   {
      const Vector2 &dst = sprite.size_;
      const float x0 = 0.0f;
      const float y0 = 0.0f;
      const float x1 = dst.x_;
      const float y1 = dst.y_;

      const float iu = 1.0f / float(material.texture_ ? material.texture_->width_ : 2.0f);
      const float iv = 1.0f / float(material.texture_ ? material.texture_->height_ : 2.0f);
      const GLuint id = material.texture_ ? material.texture_->id_ : g_font.id_;

      const Rectangle &src = sprite.source_;
      const float u0 = (src.x_              ) * iu;
      const float v0 = (src.y_              ) * iv;
      const float u1 = (src.x_ + src.width_ ) * iu;
      const float v1 = (src.y_ + src.height_) * iv;

      const float r = material.color_.r_;
      const float g = material.color_.g_;
      const float b = material.color_.b_;
      const float a = material.color_.a_;

      vertex_t vertices[] =
      {
         { x0, y0, u0, v0, r, g, b, a },
         { x1, y0, u1, v0, r, g, b, a },
         { x1, y1, u1, v1, r, g, b, a },
         { x0, y1, u0, v1, r, g, b, a },
      };

      const Matrix4 world = as_matrix(transform);
      for (auto &v : vertices) {
         Vector2 p(v.x_, v.y_);
         p = world * p;
         v.x_ = p.x_;
         v.y_ = p.y_;
      }

      opengl_set_texture(id);
      opengl_render_quads(4, vertices);
   }
} // !charlie
