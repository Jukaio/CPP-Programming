// server_app.hpp

#ifndef SERVER_APP_HPP_INCLUDED
#define SERVER_APP_HPP_INCLUDED

#include <charlie_application.hpp>

using namespace charlie;

struct ServerApp final : Application, network::IServiceListener, network::IConnectionListener {
   ServerApp();

   // note: Application
   virtual bool on_init();
   virtual void on_exit();
   virtual bool on_tick(const Time &dt);
   virtual void on_draw();

   // note: IServiceListener
   virtual void on_timeout(network::Connection *connection);
   virtual void on_connect(network::Connection *connection);
   virtual void on_disconnect(network::Connection *connection);

   // note: IConnectionListener 
   virtual void on_acknowledge(network::Connection *connection, const uint16 sequence);
   virtual void on_receive(network::Connection *connection, network::NetworkStreamReader &reader);
   virtual void on_send(network::Connection *connection, const uint16 sequence, network::NetworkStreamWriter &writer);

   Time accumulator_;
   uint32 tick_;

   gameplay::Entity entity_;
};

#endif // !SERVER_APP_HPP_INCLUDED
