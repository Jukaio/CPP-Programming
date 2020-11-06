// server_app.cc

#include "server_app.hpp"
#include <charlie_messages.hpp>
#include <cstdio>
#include <cmath>
#include <string>

const Time ServerApp::TICK_RATE = Time(1.0 / 60.0);

ServerApp::ServerApp()
   : m_ticks(0)
   , m_grid({ Vector2{ 0, 0 }, 
			  Vector2{ 16, 16 }, 
			  Vector2{ WIDTH , HEIGHT } })
   , m_waiting(*this)
   , m_playing(*this)
{


}

bool ServerApp::on_init()
{
   network_.set_allow_connections(true);
   network_.set_connection_limit(CLIENT_LIMIT); // I am part of two networks so my server always listens to both of them

   if (!network_.initialize(network::IPAddress(network::IPAddress::ANY_HOST, 54345))) {
      return false;
   }
   network_.set_send_rate(SERVER_SEND_RATE);
   network_.add_service_listener(this);

   m_state_handler.add_state(m_waiting);
   m_state_handler.add_state(m_playing);

   m_level_collection.push_back(gameplay::Tilemap{});
   gameplay::Tilemap::init(m_level_collection.back(), "assets/maps/test.xml");

   m_level_collection.push_back(gameplay::Tilemap{});
   gameplay::Tilemap::init(m_level_collection.back(), "assets/maps/test_2.xml");

   m_state_handler.set_state(m_waiting);
   return true;
}

void ServerApp::on_exit()
{
	
}

bool ServerApp::on_tick(const Time& dt)
{
	m_accumulator += dt;
	if(m_accumulator >= TICK_RATE)
	{
		m_accumulator -= TICK_RATE;
		m_ticks++;

		m_state_handler.on_tick(TICK_RATE);
	}

	return true;
}

void ServerApp::on_draw()
{
	m_state_handler.on_draw();
}

void ServerApp::on_acknowledge(network::Connection* connection,
							   const uint16 sequence)
{
	m_state_handler.on_acknowledge(connection, sequence);
}

void ServerApp::on_receive(network::Connection* connection,
						   network::NetworkStreamReader& reader)
{
	m_state_handler.on_receive(connection, reader);
}

void ServerApp::on_send(network::Connection* connection,
						const uint16 sequence,
						network::NetworkStreamWriter& writer)
{
	m_state_handler.on_send(connection, sequence, writer);
}

void ServerApp::on_timeout(network::Connection* connection)
{
	connection->set_listener(nullptr);
	m_client_list.remove_client((uint64)connection);
}

void ServerApp::on_connect(network::Connection* connection)
{
	connection->set_listener(this);
	m_client_list.add_client((uint64)connection);
}

void ServerApp::on_disconnect(network::Connection* connection)
{
	connection->set_listener(nullptr);
	m_client_list.remove_client((uint64)connection);
}

ServerApp::State::State(ServerApp& p_context, Type p_type)
	: m_context(p_context)
	, m_type(p_type)
{

}