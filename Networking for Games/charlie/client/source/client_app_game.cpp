// client_app_game.cpp

#include "client_app.hpp"
#include <charlie_messages.hpp>

ClientApp::Game::Game(ClientApp& p_context)
	: State(p_context, Type::GAME)
	, m_interpolators{ m_players[0].m_transform.position_, 
					   m_players[1].m_transform.position_, 
					   m_players[2].m_transform.position_, 
					   m_players[3].m_transform.position_ }
	, m_players{ { false, Transform{ }, Vector2{ 10.0f, 10.0f }, gameplay::Bomb{ false, Transform {}, Vector2{ 12.0f, 12.0f }, 0, gameplay::Explosion{} } },
				 { false, Transform{ }, Vector2{ 10.0f, 10.0f }, gameplay::Bomb{ false, Transform {}, Vector2{ 12.0f, 12.0f }, 0, gameplay::Explosion{} } },
				 { false, Transform{ }, Vector2{ 10.0f, 10.0f }, gameplay::Bomb{ false, Transform {}, Vector2{ 12.0f, 12.0f }, 0, gameplay::Explosion{} } },
				 { false, Transform{ }, Vector2{ 10.0f, 10.0f }, gameplay::Bomb{ false, Transform {}, Vector2{ 12.0f, 12.0f }, 0, gameplay::Explosion{} } } }
{
	using namespace network::message;
	using namespace std::placeholders;
	
	set_send_function(NETWORK_MESSAGE_INPUT_COMMAND, SEND_LAMBDA
	{
		  uint8 x = (uint8)m_players[get_context().m_id_on_server].m_transform.position_.x_;
		  uint8 y = (uint8)m_players[get_context().m_id_on_server].m_transform.position_.y_;

		  InputCommand message(m_input_bits, x, y, m_current_server_tick);
		  if(!message.write(writer))
			  assert(!"failed to write message!");
		  get_context().m_network_data[CURRENT][NetworkInformation::TOTAL_MESSAGES_SENT]++;
	});

	set_receive_function(NETWORK_MESSAGE_PLAYER_REPLICATIONS, RECEIVE_LAMBDA
	 {
		  auto& client_app = get_context();
		  
		  ReplicatePlayers message;
		  if(!message.read(reader))
			  assert(!"failed to read message!");

		  // 16 bits per vector (256 x 256)
		  uint8* replica_position_ptr = (uint8*)&message.replica_positions_;

		  const int vector_offset = 2;
		  const int x_index = 0;
		  const int y_index = 1;

		  for(int i = 0; i < 4; i++)
		  {
			  m_players[i].m_active = (message.player_active_mask_ & (1 << i));
			  m_interpolators[i].push(Vector2{ replica_position_ptr[(i * vector_offset) + x_index], 
			 								   replica_position_ptr[(i * vector_offset) + y_index] }, 
			 						   message.tick_);

			  if(uint8(i) == client_app.m_id_on_server)
				m_inputinator.push_server_state(Vector2{ replica_position_ptr[(i * vector_offset) + x_index],
															   replica_position_ptr[(i * vector_offset) + y_index] },
													  message.tick_);
		  };
		  get_context().m_network_data[CURRENT][NetworkInformation::TOTAL_MESSAGES_RECEIVED]++;
	 });

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
	
	set_receive_function(NETWORK_MESSAGE_MAP, RECEIVE_LAMBDA
	{
		network::message::MapData message;
		if(!message.read(reader))
			assert(!"failed to write message!");

		for(int i = 0; i < 16; i++)
			get_context().m_tilemap.m_transfer_buffer[i] = message.map_data_[i];
		get_context().m_network_data[CURRENT][NetworkInformation::TOTAL_MESSAGES_RECEIVED]++;
	});
	
	set_receive_function(network::message::NETWORK_MESSAGE_GAME_STATUS, RECEIVE_LAMBDA
	{
		GameStatus message;
		if(!message.read(reader))
			assert(!"failed to write message!");
		m_game_end_tick = message.game_end_tick_;
		m_game_over = message.game_over_;
	});

	set_receive_function(network::message::NETWORK_MESSAGE_BOMB, RECEIVE_LAMBDA
	{
		if(reader.peek() == NETWORK_MESSAGE_BOMB)
		{ 
			network::message::Bomb message;
			if(!message.read(reader))
				assert(!"failed to write message!");
			
			gameplay::Bomb& bomb = m_players[message.planter_id_].m_bomb;
			if(!bomb.m_active)
			{
				bomb.m_active = true;
				bomb.m_transform.set_position( Vector2{ message.x_, message.y_ } );
				bomb.m_tick_flag = message.target_tick_;
			}
			get_context().m_network_data[CURRENT][NetworkInformation::TOTAL_MESSAGES_RECEIVED]++;
		}
	});

}

void ClientApp::Game::on_enter()
{
	m_inputinator.set_context(m_players[get_context().m_id_on_server].m_transform.position_);
	m_game_over = false;
	m_game_end_tick = 0;
}

void ClientApp::Game::on_exit()
{
	m_game_over = false;
    m_game_end_tick = 0;
}

void ClientApp::Game::on_input(const Keyboard::Key& p_key,
							   const Keyboard::KeyState& p_state)
{
	switch(p_key)
	{
		case Keyboard::Key::W:
			switch(p_state)
			{
				case Keyboard::KeyState::Pressed:  
					m_input_bits |= (uint8)gameplay::Action::UP; 
					break;
				case Keyboard::KeyState::Released: 
					m_input_bits &= ~((uint8)gameplay::Action::UP); 
					break;
			}
			break;
		case Keyboard::Key::A:
			switch(p_state)
			{
				case Keyboard::KeyState::Pressed:  
					m_input_bits |= (uint8)gameplay::Action::LEFT; 
					break;
				case Keyboard::KeyState::Released: 
					m_input_bits &= ~((uint8)gameplay::Action::LEFT); 
					break;
			}
			break;
		case Keyboard::Key::S:
			switch(p_state)
			{
				case Keyboard::KeyState::Pressed:  
					m_input_bits |= (uint8)gameplay::Action::DOWN; 
					break;
				case Keyboard::KeyState::Released: 
					m_input_bits &= ~((uint8)gameplay::Action::DOWN);
					break;
			}
			break;
		case Keyboard::Key::D:
			switch(p_state)
			{
				case Keyboard::KeyState::Pressed:  
					m_input_bits |= (uint8)gameplay::Action::RIGHT; 
					break;
				case Keyboard::KeyState::Released: 
					m_input_bits &= ~((uint8)gameplay::Action::RIGHT); 
					break;
			}
			break;
		case Keyboard::Key::Space:
			switch(p_state)
			{
				case Keyboard::KeyState::Pressed:
					m_input_bits |= (uint8)gameplay::Action::BOMB;
					break;
				case Keyboard::KeyState::Released:
					m_input_bits &= ~((uint8)gameplay::Action::BOMB);
					break;
			}
			break;
	}
}

void ClientApp::Game::on_tick(const Time& dt)
{
	auto tick_difference = get_context().m_ticks - get_context().m_server_times.current().m_client;
	auto latency = uint32(get_context().m_server_connection.latency().as_milliseconds() / TICK_RATE.as_milliseconds());
	m_current_server_tick = get_context().m_server_times.current().m_server + tick_difference + latency;
	
	uint8 id = get_context().m_id_on_server;

	for(int i = 0; i < 4; i++)
	{
		if(m_players[i].m_active)
			if(uint8(i) != id)
			{
				m_interpolators[i].interpolate(m_current_server_tick, 200);
			}
			else
			{
				std::function<Vector2(uint8)> input_direction_function = [](uint8 p_input)
				{
					Vector2 direction{ 0, 0 };
					if(p_input & uint8(gameplay::Action::UP))
						direction.y_ -= 1;
					if(p_input & uint8(gameplay::Action::LEFT))
						direction.x_ -= 1;
					if(p_input & uint8(gameplay::Action::DOWN))
						direction.y_ += 1;
					if(p_input & uint8(gameplay::Action::RIGHT))
						direction.x_ += 1;
					direction.normalize();
					return direction;
				};

				Vector2 prev_pos = m_players[id].m_transform.position_;
				Vector2 direction = input_direction_function(m_input_bits);

				if(direction.length() > 0)
				{
					Vector2 next_pos = m_players[id].m_transform.position_ + direction * PLAYER_SPEED * dt.as_seconds();
					m_players[id].m_transform.set_position(next_pos);

					if(gameplay::Tilemap::collides(get_context().m_tilemap,
					   get_context().m_grid,
					   m_players[id].m_transform.position_,
					   m_players[id].m_dimension))
						 m_players[id].m_transform.set_position(prev_pos);


				}
				if(m_inputinator.input_correction(5.0f, dt, input_direction_function))
					get_context().m_network_data[CURRENT][NetworkInformation::INPUT_MISSPREDICTIONS]++;
			}

		gameplay::Bomb& bomb = m_players[i].m_bomb;

		gameplay::Bomb::update(bomb,
							   get_context().m_tilemap,
							   get_context().m_grid,
							   m_current_server_tick,
							   45);
		gameplay::Bomb::clean_explosion(bomb.m_explosion,
										m_current_server_tick);
	}

	int active_count = 0;
	for(int i = 0; i < 4; i++)
		active_count += m_players[i].m_active;
	if(!(active_count > 1))
	{
		if(m_players[get_context().m_id_on_server].m_active)
		{
			game_end_text = "YOU WON!";
		}
		else
		{
			game_end_text = "YOU LOST!";
		}
	}

	m_inputinator.push(m_players[id].m_transform.position_ ,
					   m_input_bits,
					   m_current_server_tick);

	if(m_current_server_tick > m_game_end_tick && m_game_over)
	{
		get_context().m_state_handler.set_state(get_context().m_lobby);
	}

	if(get_context().m_server_connection.is_disconnected() ||
	   get_context().m_server_connection.state_ == network::Connection::State::Invalid ||
	   get_context().m_server_connection.state_ == network::Connection::State::Timedout)
	{
		get_context().m_state_handler.set_state(get_context().m_menu_timeout);
	}

	gameplay::Tilemap::apply_received_data(get_context().m_tilemap);
}

void ClientApp::Game::on_draw()
{
	auto& client_app = get_context();

	gameplay::Tilemap::render(client_app.m_tilemap, client_app.m_grid, client_app.renderer_, SCALE);

	for(int i = 0; i < 4; i++)
	{
		gameplay::Bomb& bomb = m_players[i].m_bomb;
		if(bomb.m_active)
		{
			Rectangle rect = { int32(bomb.m_transform.position_.x_ * SCALE),
							   int32(bomb.m_transform.position_.y_ * SCALE),
							   16 * SCALE, 16 * SCALE };
			client_app.renderer_.render_rectangle_fill(rect, Color::Aqua);
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
					client_app.renderer_.render_rectangle_fill(rect, Color::Lime);
				}
			}
		}
	}


	for(int i = 0; i < 4; i++)
	{
		Color color = (i == client_app.m_id_on_server) ? Color::Blue : Color::Red;
		gameplay::Player::draw(client_app.renderer_, m_players[i], SCALE, color);
	}

	std::string server_tick_string = std::to_string(m_current_server_tick);
	client_app.renderer_.render_text({ 2, (get_context().HEIGHT * SCALE)- 16 }, Color::Green, SCALE, server_tick_string.c_str());

	if(m_game_over)
		client_app.renderer_.render_text({ WIDTH / 2, HEIGHT / 2 }, Color::Black, SCALE + 1, game_end_text.c_str());
}

void ClientApp::Game::on_acknowledge(network::Connection* connection, const uint16 sequence)
{
}
