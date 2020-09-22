// main.cc

#include <cstdio>
#include "client_app.hpp"

int main(int argc, char **argv)
{
   ClientApp app;
   if (app.init()) {
      app.run();
      app.exit();
   }

   return 0;
}
