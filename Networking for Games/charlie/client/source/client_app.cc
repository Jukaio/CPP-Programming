// client_app.cc

#include "client_app.hpp"
#include <charlie_messages.hpp>
#include <cstdio>

ClientApp::ClientApp()
   : mouse_(window_.mouse_)
   , keyboard_(window_.keyboard_)
	, m_interpolator(Time(0.1))
	, ticks_(0)
{
}

bool ClientApp::on_init()
{
   if (!network_.initialize({})) {
      return false;
   }

   connection_.set_listener(this);
   connection_.connect(network::IPAddress(192, 168, 1, 2, 54322));

   return true;
}

void ClientApp::on_exit()
{
}

bool ClientApp::on_tick(const Time &dt)
{
   if (keyboard_.pressed(Keyboard::Key::Escape)) {
      return false;
   }

   accumulator_ += dt;
   if (accumulator_ >= Time(1.0 / 60.0)) {
	   accumulator_ -= Time(1.0 / 60.0);
	   
	   ticks_++;

	   //uint32 current = m_interpolator.m_buffer.m_buffer.front().m_tick;
	   const Time difference = m_time_states.current().m_server.now - m_time_states.current().m_client.now;
	   EntityState state = m_interpolator.step(Time::now() + difference);
	   entity_.position_ = state.m_position;
	   //printf("Tick: %d -- Index: %d --pos x: %f -- pos y: %f \n", state.m_tick, m_interpolator.m_index, state.m_position.x_, state.m_position.y_);
	   
	   printf("Time: %f -- Other: %f \n", (Time::now() + difference).as_seconds(), m_time_states.current().m_server.now.as_seconds());
	   
	   //printf("Interpol count: %d \n", (int)m_interpolator.m_buffer.m_buffer.size());
	   //printf("Server: Time: %f -- dt: %f -- Ticks: %u  --- ", m_time_states.current().m_server.now.as_seconds(),
				//											   m_time_states.current().m_server.dt.as_seconds(),
				//											   (int)m_time_states.current().m_server.ticks);

	   //printf("Client: Time: %f -- dt: %f -- Ticks: %u \n ",  m_time_states.current().m_client.now.as_seconds(),
				//											   m_time_states.current().m_client.dt.as_seconds(),
				//											   (int)m_time_states.current().m_client.ticks);
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
	EntityState state;
   {
      network::NetworkMessageServerTick message;
      if (!message.read(reader)) {
         assert(!"could not read message!");
      }

	  TimeState temp;
	  temp.m_server = { Time(message.server_time_), Time(message.server_dt_) , message.server_tick_ };
	  temp.m_client = { Time::now(), Time::deltatime(), ticks_ };
	  m_time_states.push(temp);

	  state.m_tick = message.server_tick_;
	  state.m_time = message.server_time_;
   }

   {
      network::NetworkMessageEntityState message;
      if (!message.read(reader)) {
         assert(!"could not read message!");
      }
	  state.m_position = message.position_;
   }
   m_interpolator.push(state);

   //printf("NFO: %llu %u\n", 
   //       message.server_time_, 
   //       message.server_tick_);
}

void ClientApp::on_send(network::Connection *connection, 
                        const uint16 sequence, 
                        network::NetworkStreamWriter &writer)
{
	{
		network::NetworkMessageClientTick message(Time::now().as_ticks(),
												  ticks_,
												  Time::deltatime().as_ticks());
		if (!message.write(writer)) {
			assert(!"failed to write message!");
		}
	}
}
