// client_app_lobby.cpp

#include <limits>
#include "client_app.hpp"
#include "charlie_messages.hpp"

void ClientApp::Lobby::receive_server_status(network::Connection* connection, network::NetworkStreamReader& reader)
{
	network::message::ServerStatus message;
	if(!message.read(reader))
		assert(!"failed to read message!");
	m_server_status = message.ready_;
	m_player_count = message.player_count_;
	m_start_game_tick = message.game_start_tick_;
	get_context().m_id_on_server = message.client_id_;
	get_context().m_network_data[CURRENT][NetworkInformation::TOTAL_MESSAGES_RECEIVED]++;
}

void ClientApp::Lobby::send_client_ready(network::Connection* connection, const uint16 sequence, network::NetworkStreamWriter& writer)
{
	network::message::ClientReady message(m_ready);
	if(!message.write(writer))
	{
		assert(!"failed to write message!");
	}
	get_context().m_network_data[CURRENT][NetworkInformation::TOTAL_MESSAGES_SENT]++;
}


ClientApp::Lobby::Lobby(ClientApp& p_context)
	: State(p_context, Type::LOBBY)
	, m_server_tick(0)
	, m_start_game_tick(std::numeric_limits<uint32>::max())
{
	using namespace network::message;
	using namespace std::placeholders;

	set_send_function(NETWORK_MESSAGE_CLIENT_READY,
					  std::bind(&ClientApp::Lobby::send_client_ready, this, _1, _2, _3));

	set_receive_function(NETWORK_MESSAGE_SERVER_STATUS,
						 std::bind(&ClientApp::Lobby::receive_server_status, this, _1, _2));

	set_send_function(NETWORK_MESSAGE_TICK, SEND_LAMBDA
	{
		network::message::Tick message(get_context().m_ticks);
		if(!message.write(writer))
			assert(!"failed to write message!");
		get_context().m_network_data[CURRENT][NetworkInformation::TOTAL_MESSAGES_SENT]++;
	});

	set_receive_function(NETWORK_MESSAGE_TICK, RECEIVE_LAMBDA
	{
		network::message::Tick message;
		if(!message.read(reader))
			assert(!"failed to read message!");
		TimeState temp{ get_context().m_ticks, message.tick_ };
		get_context().m_server_times.push(temp);
		get_context().m_network_data[CURRENT][NetworkInformation::TOTAL_MESSAGES_RECEIVED]++;
	});


}

void ClientApp::Lobby::on_enter()
{
	m_start_game_tick = std::numeric_limits<uint32>::max();
	m_ready = false;
	m_server_status = false;
}

void ClientApp::Lobby::on_exit()
{
	m_start_game_tick = std::numeric_limits<uint32>::max();
	m_ready = false;
	m_server_status = false;
}

void ClientApp::Lobby::on_input(const Keyboard::Key& p_key, const Keyboard::KeyState& p_state)
{
	switch(p_key)
	{
		case Keyboard::Key::Enter:
			switch(p_state)
			{
				case Keyboard::KeyState::Pressed:
					m_ready = !m_ready;
					break;
			}
			break;

		case Keyboard::Key::Back:
			switch(p_state)
			{
				case Keyboard::KeyState::Pressed:
					get_context().m_server_connection.disconnect();
					break;
			}
			break;
	}
}

void ClientApp::Lobby::on_tick(const Time& dt)
{
	auto tick_difference = get_context().m_ticks - get_context().m_server_times.current().m_client;
	auto latency = uint32(get_context().m_server_connection.latency().as_milliseconds() / TICK_RATE.as_milliseconds());
	m_server_tick = get_context().m_server_times.current().m_server + tick_difference + latency;

	if(m_server_tick > m_start_game_tick && m_server_status)
	{
		get_context().m_state_handler.set_state(get_context().m_game);
	}

	if(get_context().m_server_connection.is_disconnected() ||
	   get_context().m_server_connection.state_ == network::Connection::State::Invalid ||
	   get_context().m_server_connection.state_ == network::Connection::State::Timedout)
	{
		get_context().m_state_handler.set_state(get_context().m_menu_timeout);
	}

}

void ClientApp::Lobby::on_draw()
{
	ClientApp& client_app = get_context();
	//gameplay::Grid::draw(client_app.m_grid, client_app.renderer_, Color::Aqua);


	std::string ready_text = (m_ready) ? "I AM READY!" : "I AM NOT READY!";
	std::string server_text = (m_server_status) ? "SERVER IS READY!" : "SERVER IS NOT READY!";
	Color ready_color = (m_ready) ? Color::Green : Color::Red;
	Color server_color = (m_server_status) ? Color::Green : Color::Red;

	client_app.renderer_.render_text({ int32(client_app.window_.width_ * 0.25f),
									   int32(client_app.window_.height_ * 0.60f) }, 
									   server_color, SCALE, server_text.c_str());
	client_app.renderer_.render_text({ int32(client_app.window_.width_ * 0.25f),
									   int32(client_app.window_.height_ * 0.65f) }, ready_color, SCALE, ready_text.c_str());

	std::string player_count_text = "Connected Players: " + std::to_string(m_player_count);

	client_app.renderer_.render_text({ int32(client_app.window_.width_ * 0.25f),
									   int32(client_app.window_.height_ * 0.70f) }, server_color, SCALE, player_count_text.c_str());

	std::string server_tick_string = std::to_string(m_server_tick);
	client_app.renderer_.render_text({ get_context().window_.width_ - 60 - (int32(server_tick_string.size()) * 8), int32(client_app.window_.height_ * 0.95f) }, server_color, SCALE, server_tick_string.c_str());
	client_app.renderer_.render_text({ get_context().window_.width_ - 60 - (int32(server_tick_string.size()) * 8), int32(client_app.window_.height_ * 0.80f) }, Color::Lime, SCALE, std::to_string(m_start_game_tick).c_str());

}

void ClientApp::Lobby::on_acknowledge(network::Connection* connection, const uint16 sequence)
{
}
