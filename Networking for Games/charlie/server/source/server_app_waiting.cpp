// server_app_waiting.cpp

#include "server_app.hpp"
#include "charlie_messages.hpp"

ServerApp::Waiting::Waiting(ServerApp& p_context)
	: State(p_context, Type::WAITING)
{
	using namespace network::message;

	set_send_function(NETWORK_MESSAGE_SERVER_STATUS, SEND_LAMBDA
	{
		network::message::ServerStatus message(m_game_starts, 
											   uint8(get_context().m_client_list.m_clients.size()), 
											   uint8(get_context().m_client_list.find_client(uint64(connection))->m_id),
											   m_game_start_tick);
		if(!message.write(writer))
			assert(!"failed to write message!");
	});

	set_send_function(NETWORK_MESSAGE_TICK, SEND_LAMBDA
	{
		network::message::Tick message(get_context().m_ticks);
		if(!message.write(writer))
			assert(!"failed to write message!");
	});

	set_receive_function(NETWORK_MESSAGE_CLIENT_READY, RECEIVE_LAMBDA
	{
		network::message::ClientReady message;
		if(!message.read(reader))
			assert(!"failed to read message!");
		m_player_states[get_context().m_client_list.find_client(uint64(connection))->m_id] = PlayerState(!!message.ready_);
	});

	set_receive_function(NETWORK_MESSAGE_TICK, RECEIVE_LAMBDA
	{
		network::message::Tick message;
		if(!message.read(reader))
			assert(!"failed to write message!");
		TimeState temp { message.tick_, get_context().m_ticks };
		get_context().m_client_list.find_client(uint64(connection))->m_time_state.push(temp);
	});
}

bool ServerApp::Waiting::ready_to_play()
{
	bool all_ready = (get_context().m_client_list.m_clients.size() >= MINIMUM_CLIENTS_TO_START);
	for(auto& client : get_context().m_client_list.m_clients)
	{
		if(m_player_states[client.m_id] == NOT_READY)
			all_ready = false;
	}
	return all_ready;
}

void ServerApp::Waiting::on_enter()
{

}

void ServerApp::Waiting::on_exit()
{
	for(auto& client : get_context().m_client_list.m_clients)
	{
		m_player_states[client.m_id] = NOT_READY;
	}
}

void ServerApp::Waiting::on_input(const Keyboard::Key& p_key, const Keyboard::KeyState& p_state)
{

}

void ServerApp::Waiting::on_tick(const Time& dt)
{
	if(ready_to_play())
	{
		//get_context().m_state_handler.set_state(get_context().m_playing);
		m_game_starts = true;
	}
	else
	{
		m_game_start_tick = get_context().m_ticks + (60 * 5);
		m_game_starts = false;
	}

	if(get_context().m_ticks > m_game_start_tick)
	{
		get_context().m_state_handler.set_state(get_context().m_playing);
	}
}

void ServerApp::Waiting::on_draw()
{
	ServerApp& server_app = get_context();
	
	for(int i = 0; i < server_app.m_client_list.m_clients.size(); i++)
	{
		server_app.renderer_.render_text({4, 10 + (20 * i)}, Color::Red, 2, (std::string("Client: ") + std::to_string(server_app.m_client_list.m_clients[i].m_id)).c_str());
	}

	for(int i = 0; i < server_app.m_client_list.m_clients.size(); i++)
	{
		server_app.renderer_.render_text({ 172, 10 + (20 * i) }, Color::Red, 2, std::to_string(m_player_states[i]).c_str());
	}

	get_context().renderer_.render_text({ 200, 600 }, 
										Color::Red, 
										2, 
										std::to_string(m_game_start_tick).c_str());

	
	server_app.renderer_.render_text({ 100, 300 }, Color::Red, 2, std::to_string(get_context().m_ticks).c_str());
	
	if(get_context().m_client_list.m_clients.size() > 0)
		server_app.renderer_.render_text({ 200, 300 }, Color::Red, 2, std::to_string(get_context().m_client_list.m_clients[0].m_time_state.current().m_server).c_str());

}

void ServerApp::Waiting::on_acknowledge(network::Connection* connection, const uint16 sequence)
{
}


