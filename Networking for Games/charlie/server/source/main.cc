// main.cc

#include <cstdio>
#include "server_app.hpp"

int main(int argc, char **argv)
{
   ServerApp app;
   if (app.init()) {
      app.run();
      app.exit();
   }

   return 0;
}
