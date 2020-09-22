// charlie_application.cc

#include "charlie_application.hpp"

namespace charlie {
   Application::Application()
   {
   }

   Application::~Application()
   {
   }

   bool Application::init()
   {
      if (!window_.open(Settings("charlie", 640, 360))) {
         return false;
      }

      if (!renderer_.initialize(window_)) {
         return false;
      }

      return on_init();
   }

   void Application::run()
   {
      bool running = true;
      while (running) {
         if (!window_.update()) {
            running = false;
         }
         network_.update();

         const auto dt = Time::deltatime();
         if (!on_tick(dt)) {
            running = false;
         }

         renderer_.clear(Color::Black);
         on_draw();
         window_.present();
      }
   }

   void Application::exit()
   {
      on_exit();

      network_.shutdown();
      renderer_.shutdown();
      window_.close();
   }

   bool Application::on_init()
   {
      return true;
   }

   void Application::on_exit()
   {
   }

   bool Application::on_tick(const Time &dt)
   {
      return true;
   }

   void Application::on_draw()
   {
   }
} // !charlie
