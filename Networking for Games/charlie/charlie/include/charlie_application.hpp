// charlie_application.hpp

#ifndef CHARLIE_APPLICATION_HPP_INCLUDED
#define CHARLIE_APPLICATION_HPP_INCLUDED

#include <charlie.hpp>
#include <charlie_network.hpp>
#include <charlie_gameplay.hpp>

namespace charlie {
   struct Application {
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
