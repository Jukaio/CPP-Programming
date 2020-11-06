// client_app_menu.cpp

#include "client_app.hpp"
#include <charlie_messages.hpp>

ClientApp::Menu::Menu(ClientApp& p_context)
	: State(p_context, Type::MENU)
{
	auto& client_app = get_context();
}

void ClientApp::Menu::on_enter()
{
	m_index = 0;
}

void ClientApp::Menu::on_exit()
{
}

void ClientApp::Menu::on_input(const Keyboard::Key& p_key,
							   const Keyboard::KeyState& p_state)
{
	switch(p_key)
	{
		case Keyboard::Key::W:
			if(p_state == Keyboard::KeyState::Pressed)
				m_index = (m_index + MENU_POINTS - 1) % MENU_POINTS;
			break;

		case Keyboard::Key::S:
			if(p_state == Keyboard::KeyState::Pressed)
				m_index = (m_index + 1) % MENU_POINTS;
			break;

		case Keyboard::Key::Enter:
			if(p_state == Keyboard::KeyState::Pressed)
			{
				switch(MenuTransitions(m_index))
				{
					case MenuTransitions::FIND_SERVER:
						get_context().m_state_handler.set_state(get_context().m_find_server);
						break;
					case MenuTransitions::EXIT:
						get_context().m_exit = true;
						break;
				}
			}
	}
}

void ClientApp::Menu::on_tick(const Time& dt)
{	

}

void ClientApp::Menu::on_draw()
{
   ClientApp& client_app = get_context();
   

   for(int i = 0; i < MENU_POINTS; i++)
   {
	   Color color = (i != m_index) ? Color::White : Color::Red;
	   client_app.renderer_.render_text({ int32(client_app.window_.width_ * 0.25f), 
										  int32(((client_app.window_.height_ * 0.60f) + (i * 60))) }, 
										  color, 
										  SCALE, 
										  m_menu_text[i].c_str());

   }
}

void ClientApp::Menu::on_acknowledge(network::Connection* connection, const uint16 sequence)
{
}
