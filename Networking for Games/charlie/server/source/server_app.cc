// server_app.cc

#include "server_app.hpp"
#include <charlie_messages.hpp>
#include <cstdio>
#include <cmath>

ServerApp::ServerApp()
   : tick_(0)
{
}

bool ServerApp::on_init()
{
   network_.set_send_rate(Time(1.0 / 2.0));
   network_.set_allow_connections(true);
   if (!network_.initialize(network::IPAddress(9, 0, 0, 1, 54345))) {
      return false;
   }

   network_.add_service_listener(this);

   entity_.position_ = { 300.0f, 200.0f };

   return true;
}

void ServerApp::on_exit()
{
}

bool ServerApp::on_tick(const Time &dt)
{
   accumulator_ += dt;
   if (accumulator_ >= Time(1.0 / 60.0)) {
      accumulator_ -= Time(1.0 / 60.0);

      tick_++;

      entity_.position_.x_ = 300.0f + std::cosf(Time::now().as_seconds()) * 150.0f;
   }

   return true;
}

void ServerApp::on_draw()
{
   renderer_.render_text({ 2, 2 }, Color::Aqua, 1, "SERVER");

   renderer_.render_rectangle_fill(
      { 
         static_cast<int32>(entity_.position_.x_), 
         static_cast<int32>(entity_.position_.y_), 
         20, 
         20 
      }, 
      Color::Red);
}

void ServerApp::on_timeout(network::Connection *connection)
{
   connection->set_listener(nullptr);
}

void ServerApp::on_connect(network::Connection *connection)
{
   connection->set_listener(this);
}

void ServerApp::on_disconnect(network::Connection *connection)
{
   connection->set_listener(nullptr);
}

void ServerApp::on_acknowledge(network::Connection *connection, 
                               const uint16 sequence)
{
}

void ServerApp::on_receive(network::Connection *connection, 
                           network::NetworkStreamReader &reader)
{
}

void ServerApp::on_send(network::Connection *connection, 
                        const uint16 sequence, 
                        network::NetworkStreamWriter &writer)
{
   {
      network::NetworkMessageServerTick message(Time::now().as_ticks(),
                                                tick_);
      if (!message.write(writer)) {
         assert(!"failed to write message!");
      }
   }

   {
      network::NetworkMessageEntityState message(entity_.position_);
      if (!message.write(writer)) {
         assert(!"failed to write message!");
      }
   }
}
