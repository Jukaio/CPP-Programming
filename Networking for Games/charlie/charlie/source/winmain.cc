// winmain.cc

#include <Windows.h>

extern int main(int argc, char **argv);

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR command_line, int command_show)
{
   return main(__argc, __argv);
}
