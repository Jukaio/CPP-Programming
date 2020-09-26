// server_app.cc

#include "server_app.hpp"
#include <charlie_messages.hpp>
#include <cstdio>
#include <cmath>


ServerApp::ServerApp()
   : ticks_(0)
{
}

bool ServerApp::on_init()
{
   network_.set_send_rate(Time(1.0 / 2.0));
   network_.set_allow_connections(true);
   if (!network_.initialize(network::IPAddress(192, 168, 1, 2, 54322))) {
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

      ticks_++;

      entity_.position_.x_ = 300.0f + std::cosf(Time::now().as_seconds()) * 150.0f;
	  entity_.position_.y_ = 200.0f + std::sinf(Time::now().as_seconds()) * 80.0f;


	  //printf("Tick: %d Pos x: %f \n", tick_, entity_.position_.x_);
	  for (auto& client : m_client_list.m_clients)
	  {
		  //printf("Client %d: Time: %f -- dt: %f -- Ticks: %u --- ", client.m_id, client.m_time_state.current().m_client.now.as_seconds(),
				//												client.m_time_state.current().m_client.dt.as_seconds(),
				//												(int)client.m_time_state.current().m_client.ticks);

		  //printf("Server: Time: %f -- dt: %f -- Ticks: %u  \n ", client.m_time_state.current().m_server.now.as_seconds(),
				//												  client.m_time_state.current().m_server.dt.as_seconds(),
				//											      (int)client.m_time_state.current().m_server.ticks);

	  }
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
   m_client_list.remove_client((uint64)connection);
}

void ServerApp::on_connect(network::Connection *connection)
{
   connection->set_listener(this);
   m_client_list.add_client((uint64)connection);
}

void ServerApp::on_disconnect(network::Connection *connection)
{
   connection->set_listener(nullptr);
   m_client_list.remove_client((uint64)connection);
}

void ServerApp::on_acknowledge(network::Connection *connection, 
                               const uint16 sequence)
{
}

void ServerApp::on_receive(network::Connection *connection, 
                           network::NetworkStreamReader &reader)
{
	network::NetworkMessageClientTick message;
	if (!message.read(reader)) {
		assert(!"could not read message!");
	}
	auto client = m_client_list.find_client((uint64)connection);
	
	TimeState temp;
	temp.m_client = { Time(message.client_time_), Time(message.client_dt_) , message.client_tick_ };
	temp.m_server = { Time::now(), Time::deltatime(), ticks_ };
	client->m_time_state.push(temp);
}

void ServerApp::on_send(network::Connection *connection, 
                        const uint16 sequence, 
                        network::NetworkStreamWriter &writer)
{
   {
      network::NetworkMessageServerTick message(Time::now().as_ticks(),
                                                ticks_,
												Time::deltatime().as_ticks());
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
