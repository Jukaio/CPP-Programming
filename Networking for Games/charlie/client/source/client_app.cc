// client_app.cc

#include "client_app.hpp"
#include <charlie_messages.hpp>
#include <cstdio>

ClientApp::ClientApp()
   : mouse_(window_.mouse_)
   , keyboard_(window_.keyboard_)
   , m_latest_server_time(0.0)
{
}

bool ClientApp::on_init()
{
   if (!network_.initialize({})) {
      return false;
   }

   connection_.set_listener(this);
   connection_.connect(network::IPAddress(9, 0, 0, 1, 54345));

   return true;
}

void ClientApp::on_exit()
{
}


bool cmpv2(Vector2 A, Vector2 B, float epsilon = 0.005f)
{
    return Vector2::distance(A, B) == 0;
}

bool ClientApp::on_tick(const Time &dt)
{
   if (keyboard_.pressed(Keyboard::Key::Escape)) {
      return false;
   }

   accumulator_ += dt;
   if (accumulator_ >= Time(1.0 / 60.0)) {
      accumulator_ -= Time(1.0 / 60.0);

      if(buffer_.size() > 1)
      {
         //m_latest_server_time += Time::now();

         const float from = buffer_[0].time_.as_seconds();
         const float to = buffer_[1].time_.as_seconds();

         const float current_difference = Time::now().as_seconds() - to;
         const float total_difference = to - from;

         const float time_fraction = current_difference / total_difference;

         entity_.position_ = Vector2::lerp(buffer_[0].position_,
                                           buffer_[1].position_, 
                                           time_fraction);
        //printf("%d\n", (int) buffer_.size());
         printf("x: %f -- y: %f -- current diff: %f\n", entity_.position_.x_, entity_.position_.y_, current_difference);
         if(cmpv2(entity_.position_, buffer_[1].position_))
            buffer_.erase(buffer_.begin());
      }

      // note: entity interpolation goes here
   }

   return true;
}

void ClientApp::on_draw()
{
   renderer_.render_text({ 2, 2 }, Color::White, 1, "CLIENT");
   renderer_.render_rectangle_fill({ int32(entity_.position_.x_), int32(entity_.position_.y_), 20, 20 }, Color::Green);
}

void ClientApp::on_acknowledge(network::Connection *connection, 
                               const uint16 sequence)
{
}

void ClientApp::on_receive(network::Connection *connection, 
                           network::NetworkStreamReader &reader)
{
   {
      network::NetworkMessageServerTick message;
      if (!message.read(reader)) {
         assert(!"could not read message!");
      }

      m_latest_server_time = Time(message.server_time_);
   }

   {
      network::NetworkMessageEntityState message;
      if (!message.read(reader)) {
         assert(!"could not read message!");
      }
      
      buffer_.push_back({m_latest_server_time, message.position_});
   
   }

   //printf("NFO: %llu %u\n", 
   //       message.server_time_, 
   //       message.server_tick_);
}

void ClientApp::on_send(network::Connection *connection, 
                        const uint16 sequence, 
                        network::NetworkStreamWriter &writer)
{
}
