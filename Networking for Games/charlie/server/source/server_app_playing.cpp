// server_app_playing.cpp

#include "server_app.hpp"
#include "charlie_messages.hpp"



ServerApp::Playing::Playing(ServerApp& p_context)
	: State(p_context, Type::PLAYING)
	, m_players{ { false, Transform{ }, Vector2{ 10.0f, 10.0f }, gameplay::Bomb{ false, Transform {}, Vector2{ 12.0f, 12.0f }, 0, gameplay::Explosion{} } },
			     { false, Transform{ }, Vector2{ 10.0f, 10.0f }, gameplay::Bomb{ false, Transform {}, Vector2{ 12.0f, 12.0f }, 0, gameplay::Explosion{} } },
			     { false, Transform{ }, Vector2{ 10.0f, 10.0f }, gameplay::Bomb{ false, Transform {}, Vector2{ 12.0f, 12.0f }, 0, gameplay::Explosion{} } },
			     { false, Transform{ }, Vector2{ 10.0f, 10.0f }, gameplay::Bomb{ false, Transform {}, Vector2{ 12.0f, 12.0f }, 0, gameplay::Explosion{} } } }
{
	using namespace network::message;

	set_receive_function(NETWORK_MESSAGE_INPUT_COMMAND, RECEIVE_LAMBDA
	{
		auto& server_app = get_context();
		InputCommand message;
		if(!message.read(reader))
			assert(!"failed to write message!");

		int id = server_app.m_client_list.find_client(uint64(connection))->m_id;
		m_inputs[id] = message.bits_;
		m_position_tick_buffer[id].push({ message.tick_, Vector2{ message.pos_x_, message.pos_y_ }});
	});

	set_send_function(NETWORK_MESSAGE_PLAYER_REPLICATIONS, SEND_LAMBDA
	{
		// 16 bits per vector (256 x 256)
		uint8 active_mask = 0;
		uint64 replica_positions = 0;
		uint8* replica_position_ptr = (uint8*) &replica_positions;
		auto& server_app = get_context();

		for(auto& client : server_app.m_client_list.m_clients)
		{
			m_players[client.m_id];
			active_mask |= ((1 << client.m_id) * m_players[client.m_id].m_active);
			replica_position_ptr[(client.m_id * 2) + 0] = (uint8)m_players[client.m_id].m_transform.position_.x_;
			replica_position_ptr[(client.m_id * 2) + 1] = (uint8)m_players[client.m_id].m_transform.position_.y_;
		}

		ReplicatePlayers message(active_mask, replica_positions, server_app.m_ticks);
		if(!message.write(writer))
			assert(!"failed to write message!");
	});

	

	set_send_function(NETWORK_MESSAGE_TICK, SEND_LAMBDA
	{
		network::message::Tick message(get_context().m_ticks);
		if(!message.write(writer))
			assert(!"failed to write message!");
	});

	set_receive_function(NETWORK_MESSAGE_TICK, RECEIVE_LAMBDA
	{
		network::message::Tick message;
		if(!message.read(reader))
			assert(!"failed to write message!");
		TimeState temp { message.tick_, get_context().m_ticks };
		get_context().m_client_list.find_client(uint64(connection))->m_time_state.push(temp);
	});

	set_send_function(NETWORK_MESSAGE_MAP, SEND_LAMBDA
	{
		network::message::MapData message(m_tilemap.m_transfer_buffer);
		if(!message.write(writer))
			assert(!"failed to write message!");
	});

	set_send_function(network::message::NETWORK_MESSAGE_BOMB, SEND_LAMBDA
	{
		int id = get_context().m_client_list.find_client(uint64(connection))->m_id;

		for(size_t i = 0; i < m_reliable_message_buffer[id].size(); i++)
		{
			 auto& message = m_reliable_message_buffer[id][i];
			 if(message->type_ == NETWORK_MESSAGE_BOMB)
			 {
				 message->acks_.push_back(sequence);
				  if(!((ReliableBomb*)(message))->m_bomb.write(writer))
					  assert(!"failed to write message!");
			 }
		}
	});
	
	set_send_function(network::message::NETWORK_MESSAGE_GAME_STATUS, SEND_LAMBDA
	{
		GameStatus message(uint8(m_game_ends), m_game_end_ticks);
		if(!message.write(writer))
			assert(!"failed to write message!");
	});
}

void ServerApp::Playing::on_enter()
{
	auto& server_app = get_context();
	Vector2 tile_half = server_app.m_grid.m_tile_size * 0.25f;

	Vector2 spawn_positions[4] =
	{
		gameplay::Grid::grid_to_world(server_app.m_grid, Vector2{ 1, 1 })  + tile_half,
		gameplay::Grid::grid_to_world(server_app.m_grid, Vector2{ 13, 12 })+ tile_half,
		gameplay::Grid::grid_to_world(server_app.m_grid, Vector2{ 13, 1 }) + tile_half,
		gameplay::Grid::grid_to_world(server_app.m_grid, Vector2{ 1, 12 }) + tile_half,
	};

	for(auto& client : server_app.m_client_list.m_clients)
	{
		m_players[client.m_id].m_active = true;
		m_players[client.m_id].m_transform.set_position(spawn_positions[client.m_id]);
	}
	m_game_ends = false;

	static int level_counter = 0;
	m_tilemap = get_context().m_level_collection[level_counter];
	level_counter = (level_counter + 1) % get_context().m_level_collection.size();
}

void ServerApp::Playing::on_exit()
{
	m_game_ends = false;
}

void ServerApp::Playing::on_input(const Keyboard::Key& p_key, const Keyboard::KeyState& p_state)
{
}

void ServerApp::Playing::on_tick(const Time& dt)
{
	auto& server_app = get_context();

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		gameplay::Bomb& bomb = m_players[i].m_bomb;
		if(m_players[i].m_active)
		{
			Vector2 prev_pos = m_players[i].m_transform.position_;

			Vector2 direction{ 0, 0 };
			if(m_inputs[i] & uint8(gameplay::Action::UP))
				direction.y_ -= 1;
			if(m_inputs[i] & uint8(gameplay::Action::LEFT))
				direction.x_ -= 1;
			if(m_inputs[i] & uint8(gameplay::Action::DOWN))
				direction.y_ += 1;
			if(m_inputs[i] & uint8(gameplay::Action::RIGHT))
				direction.x_ += 1;

			direction.normalize();
			Vector2 next_pos = m_players[i].m_transform.position_ + direction * PLAYER_SPEED * dt.as_seconds();
			m_players[i].m_transform.set_position(next_pos);

			if(gameplay::Tilemap::collides(m_tilemap,
									       server_app.m_grid,
									       m_players[i].m_transform.position_,
										   m_players[i].m_dimension))
				m_players[i].m_transform.set_position(prev_pos);
		
			if(m_inputs[i] & uint8(gameplay::Action::BOMB))
			{	
				if(!bomb.m_active && !bomb.m_explosion.m_active)
				{
					gameplay::Bomb::activate(bomb, 
											 server_app.m_grid,
											 m_players[i].m_transform.position_,
											 server_app.m_ticks,
											 90);

					for(auto& client : server_app.m_client_list.m_clients)
					{
						network::message::ReliableBomb* bomb_message = &m_reliable_bomb_bool[i];
						bomb_message->m_bomb = network::message::Bomb{ uint8(bomb.m_transform.position_.x_),
																	   uint8(bomb.m_transform.position_.y_),
																	   bomb.m_tick_flag,
																	   uint8(i) };
						m_reliable_message_buffer[client.m_id].push_back(bomb_message);
					}
				}
			}
		}

		gameplay::Bomb::update(bomb,
							   m_tilemap,
							   server_app.m_grid,
							   server_app.m_ticks,
							   45);
		gameplay::Bomb::clean_explosion(bomb.m_explosion,
										server_app.m_ticks);

	}

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(m_players[i].m_active)
		{
			Rectangle player_rect = { int32(m_players[i].m_transform.position_.x_),
									  int32(m_players[i].m_transform.position_.y_),
									  int32(m_players[i].m_dimension.x_),
									  int32(m_players[i].m_dimension.x_)};

			for(int j = 0; j < MAX_PLAYERS; j++)
			{
				
				gameplay::Bomb& bomb = m_players[j].m_bomb;
				/*
				if(bomb.m_active)
				{
					Rectangle bomb_rect = { bomb.m_transform.position_.x_,
											bomb.m_transform.position_.y_,
											bomb.m_dimension.x_,
											bomb.m_dimension.x_ };
					// each player with each bomb
					if(gameplay::collision::rect_rect_collision(player_rect, bomb_rect))
					{

					}
				}
				*/
				if(bomb.m_explosion.m_active)
				{
					auto& blasts = bomb.m_explosion.m_blasts;
					for(int k = 0; k < sizeof(blasts) / sizeof(gameplay::Blast); k++)
					{
						if(blasts[k].m_active)
						{
							Rectangle blast_rect = { int32(blasts[k].m_transform.position_.x_),
													 int32(blasts[k].m_transform.position_.y_),
													 16,
													 16 };
							if(gameplay::collision::rect_rect_collision(player_rect, blast_rect))
							{
								m_players[i].m_active = false;
							}
						}
					}
				}
			}
		}
	}


	int active_count = 0;
	for(int i = 0; i < MAX_PLAYERS; i++)
		active_count += m_players[i].m_active;

	if(!(active_count > 1))
	{
		m_game_ends = true;
	}
	else
	{
		m_game_end_ticks = get_context().m_ticks + (60 * 5);
		m_game_ends = false;
	}
	if(server_app.m_ticks >= m_game_end_ticks && m_game_ends)
	{
		server_app.m_state_handler.set_state(server_app.m_waiting);
	}

	gameplay::Tilemap::prepare_sent_data(m_tilemap);
}

void ServerApp::Playing::on_draw()
{
	auto& server_app = get_context();

	gameplay::Tilemap::render(m_tilemap, server_app.m_grid, server_app.renderer_, SCALE);

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		gameplay::Bomb& bomb = m_players[i].m_bomb;
		if(bomb.m_active)
		{
			Rectangle rect = { int32(bomb.m_transform.position_.x_ * SCALE),
							   int32(bomb.m_transform.position_.y_ * SCALE),
							   16 * SCALE, 16 * SCALE };
			server_app.renderer_.render_rectangle_fill(rect, Color::Aqua);
		}

		if(bomb.m_explosion.m_active)
		{
			for(int j = 0; j < 32; j++)
			{
				gameplay::Blast& blast = bomb.m_explosion.m_blasts[j];
				if(blast.m_active)
				{
					Rectangle rect = { int32(blast.m_transform.position_.x_ * SCALE),
									   int32(blast.m_transform.position_.y_ * SCALE),
									   16 * SCALE, 16 * SCALE };
					server_app.renderer_.render_rectangle_fill(rect, Color::Lime);
				}
			}
		}
	}

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		gameplay::Player::draw(get_context().renderer_,
							   m_players[i],
							   SCALE);
	}

	for(int p = 0; p < MAX_PLAYERS; p++)
	{
		for(int i = 0; i < 5; i++)
		{
			bool current_bit = m_inputs[p] & (1 << i);
			server_app.renderer_.render_text({ 300 + (32 * i), 400 + (32 * p) }, Color::Magenta, 2, std::to_string(current_bit).c_str());

		}
	}



	server_app.renderer_.render_text( { 0 + 2, 
									    server_app.window_.height_ - 32 },
									   Color::Green,
									   2,
									   std::to_string(server_app.m_ticks).c_str());
}

void ServerApp::Playing::on_acknowledge(network::Connection* connection, const uint16 acknowledge)
{
	int id = get_context().m_client_list.find_client(uint64(connection))->m_id;

	DynamicArray<int> to_erase;
	for(int i = 0; i < m_reliable_message_buffer[id].size(); i++)
	{
		for(int j = 0; j < m_reliable_message_buffer[id][i]->acks_.size(); j++)
		{
			if(m_reliable_message_buffer[id][i]->acks_[j] == acknowledge)
			{
				to_erase.push_back(i); 
				break;
			}
		}
	}

	std::reverse(to_erase.begin(), to_erase.end());

	for(int index = 0; index < to_erase.size(); index++)
	{
		int to_remove = to_erase[index];
		m_reliable_message_buffer[id].erase(m_reliable_message_buffer[id].begin() + to_remove);
	}
}
