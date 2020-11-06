// client_app.cc

#include "client_app.hpp"
#include <charlie_messages.hpp>
#include <cstdio>
#include <string>
#include <algorithm>
#include <limits>

const Time ClientApp::TICK_RATE = Time(1.0 / 60.0);

void difference(float& p_target, float& p_current, float& p_previous)
{
	p_target = p_current - p_previous;
}

void update_network_information_data(ClientApp& p_app, const Time& p_dt)
{
	using namespace NetworkInformation;

	auto it = p_app.m_pair_buffer.begin();
	float total = 0;
	size_t buffer_size = 0;
	float smallest = std::numeric_limits<float>::max();
	float highest = std::numeric_limits<float>::min();

	p_app.m_one_second_timer -= p_dt.as_seconds();
	if(p_app.m_one_second_timer < Time(0.0))
	{
		difference(p_app.m_network_data[ClientApp::CURRENT][KIBIBYTES_PER_SECOND_RECEIVED],
				   p_app.m_network_data[ClientApp::CURRENT][KIBIBYTES_RECEIVED],
				   p_app.m_network_data[ClientApp::PREVIOUS][KIBIBYTES_RECEIVED]);
		difference(p_app.m_network_data[ClientApp::CURRENT][KIBIBYTES_PER_SECOND_SENT],
				   p_app.m_network_data[ClientApp::CURRENT][KIBIBYTES_SENT],
				   p_app.m_network_data[ClientApp::PREVIOUS][KIBIBYTES_SENT]);
		difference(p_app.m_network_data[ClientApp::CURRENT][PACKETS_PER_SECOND_RECEIVED],
				   p_app.m_network_data[ClientApp::CURRENT][PACKETS_RECEIVED],
				   p_app.m_network_data[ClientApp::PREVIOUS][PACKETS_RECEIVED]);
		difference(p_app.m_network_data[ClientApp::CURRENT][PACKETS_PER_SECOND_SENT],
				   p_app.m_network_data[ClientApp::CURRENT][PACKETS_SENT],
				   p_app.m_network_data[ClientApp::PREVIOUS][PACKETS_SENT]);

		auto sent = p_app.m_server_connection.sequence_ - p_app.m_network_data[ClientApp::CURRENT][PACKETS_SENT];
		auto received = p_app.m_server_connection.acknowledge_ - p_app.m_network_data[ClientApp::CURRENT][PACKETS_RECEIVED];

		p_app.m_network_data[ClientApp::CURRENT][PACKET_LOSS] = sent + received;

		for(int i = 0; i < NETWORK_DATA_TYPE_COUNT; i++)
			p_app.m_network_data[ClientApp::PREVIOUS][i] = p_app.m_network_data[ClientApp::CURRENT][i];
		p_app.m_one_second_timer = Time(1.0);
	}

	while(it != p_app.m_pair_buffer.end())
	{
		auto& pair = (*it);
		if(pair.first < Time::now() - Time(1.0f))
		{
			it = p_app.m_pair_buffer.erase(it);
		}
		else
		{
			if(pair.second < smallest)
			{
				smallest = pair.second;
			}
			if(pair.second > highest)
			{
				highest = pair.second;
			}
			total += pair.second;
			it++;
		}
	}

	buffer_size = p_app.m_pair_buffer.size();

	p_app.m_pair_buffer.push_back(std::pair<Time, float>(Time::now(), p_app.m_server_connection.round_trip_time().as_milliseconds()));
	p_app.m_network_data[ClientApp::CURRENT][CURRENT_RTT] = p_app.m_pair_buffer.back().second;
	p_app.m_network_data[ClientApp::CURRENT][MAX_RTT] = highest;
	p_app.m_network_data[ClientApp::CURRENT][MIN_RTT] = smallest;
	p_app.m_network_data[ClientApp::CURRENT][AVG_RTT] = (buffer_size) ? total / buffer_size : 0;
}



ClientApp::ClientApp()
   : m_mouse(window_.mouse_)
   , m_keyboard(window_.keyboard_)
   , m_ticks(0)
   , m_network_overlay(m_network_data[CURRENT], m_network_overlay_titles)
   , m_grid({ Vector2{ 0, 0 },
		      Vector2{ 16, 16 },
			  Vector2{ WIDTH , HEIGHT } })
   , m_exit(false)
   , m_menu(*this)
   , m_game(*this)
   , m_find_server(*this)
   , m_lobby(*this)
   , m_menu_timeout(*this)
{

}

bool ClientApp::on_init()
{
   if (!network_.initialize({})) {
      return false;
   }

   network_.set_send_rate(CLIENT_SEND_RATE);

   m_state_handler.add_state(m_menu);
   m_state_handler.add_state(m_find_server);
   m_state_handler.add_state(m_lobby);
   m_state_handler.add_state(m_game);
   m_state_handler.add_state(m_menu_timeout);
   m_state_handler.set_state(m_menu);


   return true;
}

void ClientApp::on_exit()
{
}

bool ClientApp::on_tick(const Time &dt)
{
   if (m_keyboard.pressed(Keyboard::Key::Escape) || m_exit)
      return false;
   if(m_keyboard.pressed(Keyboard::Key::F11))
	   m_draw_network_overlay = !m_draw_network_overlay;

   update_network_information_data((*this), dt);

   for(uint8 i = 0; i < uint8(Keyboard::Key::Count); i++)
   {
		if(m_keyboard.down(Keyboard::Key(i)))
			m_state_handler.on_input(Keyboard::Key(i), Keyboard::KeyState::Down);
		if(m_keyboard.pressed(Keyboard::Key(i)))
			m_state_handler.on_input(Keyboard::Key(i), Keyboard::KeyState::Pressed);
		if(m_keyboard.released(Keyboard::Key(i)))
			m_state_handler.on_input(Keyboard::Key(i), Keyboard::KeyState::Released);
   }

   m_accumulator += dt;
   if (m_accumulator >= TICK_RATE) 
   {
	   m_accumulator -= TICK_RATE;
	   m_ticks++;

	   m_state_handler.on_tick(TICK_RATE);

   }

   return true;
}

void ClientApp::on_draw()
{
	m_state_handler.on_draw();

	if(m_draw_network_overlay)
		NetworkInformation::Overlay::draw(m_network_overlay, renderer_, 2, 2, 1, Color::Magenta);
}

void ClientApp::on_acknowledge(network::Connection *connection, 
                               const uint16 sequence)
{
	m_state_handler.on_acknowledge(connection, sequence);
	if(connection->sequence_ - sequence > 1)
	{
		dropped_packets++;
		m_network_data[CURRENT][NetworkInformation::PACKET_LOSS] = dropped_packets / m_network_data[CURRENT][NetworkInformation::PACKETS_SENT];
	}
}

void ClientApp::on_receive(network::Connection* connection, 
                           network::NetworkStreamReader &reader)
{
	m_network_data[CURRENT][NetworkInformation::KIBIBYTES_RECEIVED] += (reader.length() / NetworkInformation::KIBIBYTE);
	m_network_data[CURRENT][NetworkInformation::PACKETS_RECEIVED] += 1;

	m_state_handler.on_receive(connection, reader);
}

void ClientApp::on_send(network::Connection *connection, 
                        const uint16 sequence, 
                        network::NetworkStreamWriter &writer)
{
	m_network_data[CURRENT][NetworkInformation::KIBIBYTES_SENT] += (writer.length() / NetworkInformation::KIBIBYTE);
	m_network_data[CURRENT][NetworkInformation::PACKETS_SENT] += 1;

	m_state_handler.on_send(connection, sequence, writer);
}

ClientApp::State::State(ClientApp& p_context, Type p_type)
	: m_context(p_context)
	, m_type(p_type)
{
}

void ClientApp::StateHandler::add_state(State& p_state)
{
	for(int i = 0; i < m_states.size(); i++)
		assert((m_states[i]->get_type() != p_state.get_type()) && "State already exists in state list");
	m_states.push_back(&p_state);
}

void ClientApp::StateHandler::remove_state(State& p_state)
{
	for(int i = 0; i < m_states.size(); i++)
	{
		if(m_states[i]->get_type() == p_state.get_type())
		{
			m_states.erase(m_states.begin() + i);
			return;
		}
	}
}

void ClientApp::StateHandler::set_state(State& p_state)
{
	State* previous = (m_current) ? m_current : nullptr;
	for(int i = 0; i < m_states.size(); i++)
	{
		if(m_states[i]->get_type() == p_state.get_type())
		{
			m_current = m_states[i];
			if(previous) previous->on_exit();
			m_current->on_enter();
			return;
		}
	}
}

void ClientApp::StateHandler::on_input(const Keyboard::Key& p_key, const Keyboard::KeyState& p_state)
{
	if(m_current) m_current->on_input(p_key, p_state);
}

void ClientApp::StateHandler::on_tick(const Time& dt)
{
	if(m_current) m_current->on_tick(dt);
}

void ClientApp::StateHandler::on_draw()
{
	if(m_current) m_current->on_draw();

}

void ClientApp::StateHandler::on_acknowledge(network::Connection* connection, const uint16 sequence)
{
	if(m_current) m_current->on_acknowledge(connection, sequence);
}

void ClientApp::StateHandler::on_receive(network::Connection* connection, network::NetworkStreamReader& reader)
{
	if(m_current) m_current->receive_messages(connection, reader);
}

void ClientApp::StateHandler::on_send(network::Connection* connection, const uint16 sequence, network::NetworkStreamWriter& writer)
{
	if(m_current) m_current->send_messages(connection, sequence, writer);
}


// TIMEOUT MENU

ClientApp::MenuTimeout::MenuTimeout(ClientApp& p_context)
	: Menu(p_context)
{
	set_type(MENU_TIMEOUT);
}

void ClientApp::MenuTimeout::on_draw()
{
	Menu::on_draw();
	get_context().renderer_.render_text({ (WIDTH / 2) - (16 * SCALE), (HEIGHT / 2) + (8 * SCALE) }, Color::Red, SCALE, m_timeout_message.c_str());
}
