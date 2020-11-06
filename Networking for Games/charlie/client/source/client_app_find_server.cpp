// client_app_find_server.cpp

#include "client_app.hpp"

ClientApp::FindServer::FindServer(ClientApp& p_context)
	: State(p_context, Type::FIND_SERVER)
{
	m_discovery.initialize({}, 54345);
}

void ClientApp::FindServer::on_enter()
{
	m_index = 0;
	m_discovery.m_discovered_addresses.clear();
}

void ClientApp::FindServer::on_exit()
{
	m_discovery.m_discovered_addresses.clear();
}

void ClientApp::FindServer::on_input(const Keyboard::Key& p_key, const Keyboard::KeyState& p_state)
{
	auto& client_app = get_context();

	switch(p_key)
	{
		case Keyboard::Key::W:
			if(p_state == Keyboard::KeyState::Pressed)
			{ 
				int16 size = (int8)m_discovery.get_addresses().size() + 1;
				m_index = (m_index + size - 1) % size;
			}
			break;

		case Keyboard::Key::S:
			if(p_state == Keyboard::KeyState::Pressed)
			{
				int16 size = (int8)m_discovery.get_addresses().size() + 1;
				m_index = (m_index + 1) % size;
			}
			break;

		case Keyboard::Key::Enter:
			if(p_state == Keyboard::KeyState::Pressed)
			{
				if(m_index < (int8)m_discovery.get_addresses().size())
				{
					switch(client_app.m_server_connection.state_)
					{
						case network::Connection::State::Invalid:
						case network::Connection::State::Disconnected:
						case network::Connection::State::Timedout:
							if(m_discovery.found_server())
							{
								client_app.m_server_connection.connect(m_discovery.get_addresses()[m_index]);
								client_app.m_server_connection.set_listener(&client_app);
							}
							break;
					}
				}
				else
				{
					client_app.m_state_handler.set_state(client_app.m_menu);
				}
			}
	}
}

void ClientApp::FindServer::on_tick(const Time& dt)
{
	auto& client_app = get_context();
	auto key = client_app.m_server_connection.key_;

	m_discovery.discover();

	if(client_app.m_server_connection.is_connected())
	{
		client_app.m_state_handler.set_state(client_app.m_lobby);
	}
}

void ClientApp::FindServer::on_draw()
{
	auto& client_app = get_context();
	auto& server_list = m_discovery.get_addresses();

	for(int i = 0; i < server_list.size(); i++)
	{
		auto address = server_list[i];
		Color color = (i != m_index) ? Color::Blue : Color::Red;

		Point server_item_position{ int32(client_app.window_.width_ * 0.25f),
									int32(client_app.window_.height_ * 0.65f + (i * 60)) };

		client_app.renderer_.render_text(server_item_position,
										 color, 
										 SCALE, 
										 address.as_string());

	}
	Color color = (server_list.size() != m_index) ? Color::Blue : Color::Red;
	client_app.renderer_.render_text( { int32(client_app.window_.width_ * 0.25f),
										int32(client_app.window_.height_ * 0.65f) + (uint8(server_list.size()) * 60) },
									 color,
									 SCALE,
									 "Back");
}

void ClientApp::FindServer::on_acknowledge(network::Connection* connection, const uint16 sequence)
{
}
