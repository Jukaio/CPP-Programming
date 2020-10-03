// client_app.cc

#include "client_app.hpp"
#include <charlie_messages.hpp>
#include <cstdio>
#include <string>

const Time ClientApp::TICK_RATE = Time(1.0 / 60.0);

ClientApp::ClientApp()
   : mouse_(window_.mouse_)
   , keyboard_(window_.keyboard_)
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
   m_player.transform_.position_ = { 200.0f, 200.0f };

   m_entity.set_context(&m_context);

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
   if (accumulator_ >= TICK_RATE) {
	   accumulator_ -= TICK_RATE;
	   
	   ticks_++;
	   const auto difference = int64(m_time_states.current().m_server.ticks) - int64(m_time_states.current().m_client.ticks);
	   m_synced_ticks = int64(ticks_) + difference;


	   m_entity.update((uint32)m_synced_ticks);
	   uint8 input_bits = 0;
	   if (keyboard_.down(Keyboard::Key::W))
		   input_bits |= (uint8) gameplay::Action::UP;
	   if (keyboard_.down(Keyboard::Key::S))
		   input_bits |= (uint8)gameplay::Action::DOWN;
	   if (keyboard_.down(Keyboard::Key::D))
		   input_bits |= (uint8)gameplay::Action::RIGHT;
	   if (keyboard_.down(Keyboard::Key::A))
		   input_bits |= (uint8)gameplay::Action::LEFT;

	   m_inputs.push(input_bits);

	   Vector2 direction = Vector2{ 0, 0 };
	   if (m_inputs.current() & uint8(gameplay::Action::UP))
	   {
		   direction.y_ -= 1.0f;
	   }
	   if (m_inputs.current() & uint8(gameplay::Action::DOWN))
	   {
		   direction.y_ += 1.0f;
	   }
	   if (m_inputs.current() & uint8(gameplay::Action::RIGHT))
	   {
		   direction.x_ += 1.0f;
	   }
	   if (m_inputs.current() & uint8(gameplay::Action::LEFT))
	   {
		   direction.x_ -= 1.0f;
	   }

	   const float speed = 100.0f;
	   {
		   direction.normalize();
		   m_player.transform_.position_ += direction * speed * TICK_RATE.as_seconds();
	   }
   }

   return true;
}

void ClientApp::on_draw()
{
   renderer_.render_text({ 2, 2 }, Color::White, 1, "CLIENT");
   
   renderer_.render_text({ 2, 12 }, Color::White, 1, std::to_string(m_synced_ticks).c_str());
   
   m_entity.render(renderer_);
   renderer_.render_rectangle_fill(
	   {
		  static_cast<int32>(m_player.transform_.position_.x_),
		  static_cast<int32>(m_player.transform_.position_.y_),
		  20,
		  20
	   },
	   Color::Magenta);
   //renderer_.render_rectangle_fill({ int32(entity_.position_.x_), int32(entity_.position_.y_), 20, 20 }, Color::Green);
}

void ClientApp::on_acknowledge(network::Connection *connection, 
                               const uint16 sequence)
{
}

void ClientApp::on_receive(network::Connection *connection, 
                           network::NetworkStreamReader &reader)
{
	EntityState state;
	InputinatorState input_state;

   {
      network::NetworkMessageServerTick message;
      if (!message.read(reader)) {
         assert(!"could not read message!");
      }

	  TimeState temp;
	  temp.m_server = { Time(message.server_time_) + connection_.latency(), message.server_tick_ };
	  temp.m_client = { Time::now(), ticks_ }; // Remove dt
	  m_time_states.push(temp);

	  state.m_tick = message.server_tick_;
	  input_state.m_tick = message.server_tick_;
   }

   {
      network::NetworkMessageEntityState message;
      if (!message.read(reader)) {
         assert(!"could not read message!");
      }
	  state.m_position = message.position_;
   }

   {
	   network::NetworkMessagePlayerState message;
	   if (!message.read(reader)) {
		   assert(!"failed to write message!");
	   }
	   input_state.m_position = message.position_;
   }

   input_state.m_input = m_inputs.current();
   m_entity.push_entity_state(state);

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
												  ticks_);
		if (!message.write(writer)) {
			assert(!"failed to write message!");
		}
	}

	{
		network::NetworkMessageInputCommand message(m_inputs.current());
		if (!message.write(writer)) {
			assert(!"failed to write message!");
		}
	}
}
