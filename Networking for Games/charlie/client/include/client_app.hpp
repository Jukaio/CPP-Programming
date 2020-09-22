// client_app.hpp

#ifndef CLIENT_APP_HPP_INCLUDED
#define CLIENT_APP_HPP_INCLUDED

#include <charlie_application.hpp>

using namespace charlie;

struct ClientApp final : Application, network::IConnectionListener {
   ClientApp();

   // note: Application
   virtual bool on_init();
   virtual void on_exit();
   virtual bool on_tick(const Time &dt);
   virtual void on_draw();

   // note: IConnectionListener 
   virtual void on_acknowledge(network::Connection *connection, const uint16 sequence);
   virtual void on_receive(network::Connection *connection, network::NetworkStreamReader &reader);
   virtual void on_send(network::Connection *connection, const uint16 sequence, network::NetworkStreamWriter &writer);

   Mouse &mouse_;
   Keyboard &keyboard_;
   network::Connection connection_;
   Time accumulator_;


   gameplay::Entity entity_;
   Time m_latest_server_time;


   struct EntityState {
      Time time_;
      Vector2 position_;
   };
   DynamicArray<EntityState> buffer_;
};

#endif // !CLIENT_APP_HPP_INCLUDED
