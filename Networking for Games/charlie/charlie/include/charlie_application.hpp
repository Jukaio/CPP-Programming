// charlie_application.hpp

#ifndef CHARLIE_APPLICATION_HPP_INCLUDED
#define CHARLIE_APPLICATION_HPP_INCLUDED

#include <charlie.hpp>
#include <charlie_network.hpp>
#include <charlie_gameplay.hpp>

const charlie::Time SERVER_SEND_RATE = 1.0 / 20.0f;
const charlie::Time CLIENT_SEND_RATE = 1.0 / 60.0f;

const static float PLAYER_SPEED = 40.0f;

namespace charlie {
   struct Application 
   {
      const static int WIDTH = 256;
      const static int HEIGHT = 224;
      const static int SCALE = 2; 
      

      Application();
      virtual ~Application();

      bool init();
      void run();
      void exit();

      virtual bool on_init();
      virtual void on_exit();
      virtual bool on_tick(const Time &dt);
      virtual void on_draw();

      Window window_;
      Renderer renderer_;
      network::Service network_;
   };
} // !charlies

#endif // !CHARLIE_APPLICATION_HPP_INCLUDED
