// client_app.hpp

#ifndef CLIENT_APP_HPP_INCLUDED
#define CLIENT_APP_HPP_INCLUDED

#include <charlie_application.hpp>
#include <network_information.h>

using namespace charlie;

typedef RingBuffer<TimeState, 64> TimeStatesBuffer;

struct ClientApp final : Application, network::IConnectionListener
{
	ClientApp();
	ClientApp(const ClientApp&) = delete;
	ClientApp& operator=(const ClientApp&) = delete;

	struct EntityState
	{
		EntityState()
			: m_tick(0)
			, m_position(0.0f, 0.0f)
		{

		}
		uint32 m_tick;		// order and identificator
		Vector2 m_position;  // the position
	};

	struct PositionTimeState
	{
		Vector2 m_state;
		uint32 m_tick;
	};

	struct PositionInputTimeState
	{
		Vector2 m_state;
		uint8 m_input;
		uint32 m_tick;
	};

	struct PostionInputinator
	{
		PostionInputinator()
		{
			
		}

		void push(Vector2 p_state, uint8 p_input, uint32 p_tick)
		{
			PositionInputTimeState state;
			state.m_state = p_state;
			state.m_tick = p_tick;
			state.m_input = p_input;

			m_position_input_tick_buffer.push(state);
		}

		void set_context(Vector2& p_position)
		{
			m_position = &p_position;
		}

		void push_server_state(Vector2 p_state, uint32 p_tick)
		{
			PositionTimeState state;
			state.m_state = p_state;
			state.m_tick = p_tick;

			m_server_position_tick_buffer.push(state);
		}

		bool input_correction(float p_correction_distance, const Time& p_dt, std::function<Vector2(uint8)> p_input_direction_function)
		{
			Vector2* prediction = nullptr;
			int steps_back = 0;

			for(int j = -1; j < 63; j++) // -1 == current 
			{
				if(m_server_position_tick_buffer.current().m_tick == m_position_input_tick_buffer.previous(j).m_tick)
				{
					prediction = &m_position_input_tick_buffer.previous(j).m_state;
					steps_back = j;
					break;
				}

			}

			if(prediction)
			{
				PositionTimeState& server_state = m_server_position_tick_buffer.current();
				float distance = Vector2::distance(*prediction, server_state.m_state);
				if(distance >= p_correction_distance)
				{
					// change the corresponding buffer state to the actual server state 
					// If steps back == -1, then the for loop gets skipped since previous(-1) == current
					m_position_input_tick_buffer.previous(steps_back).m_state = server_state.m_state;

					// Use the first changed position for the simulation of the next position
					Vector2 prev_position = m_position_input_tick_buffer.previous(steps_back).m_state;
					for(int k = steps_back - 1; k >= -1; k--)
					{
						PositionInputTimeState& temp = m_position_input_tick_buffer.previous(k);
						Vector2 direction = p_input_direction_function(temp.m_input);

						if(direction.length() > 0) // Simmulate next position in the buffer
							temp.m_state = prev_position + (direction * PLAYER_SPEED * p_dt.as_seconds());

						prev_position = temp.m_state;
					}
					// Apply the last corrected value to the current position of the player
					*m_position = m_server_position_tick_buffer.current().m_state;
					return true;
				}
			}
			return false;
		}


	private:
		// I changed this, if something is buggy, revert to git | From David to David
		RingBuffer<PositionTimeState, 64> m_server_position_tick_buffer;
		RingBuffer<PositionInputTimeState, 64> m_position_input_tick_buffer;
		Vector2* m_position = nullptr;

	};

	struct PositionInterpolator
	{
		PositionInterpolator(Vector2& p_position)
			: m_position(p_position)
		{

		}

		void push(Vector2 p_state, uint32 p_tick)
		{
			PositionTimeState state;
			state.m_state = p_state;
			state.m_tick = p_tick;

			m_buffer.push(state);
		}

		void interpolate(uint32 p_sync_tick, uint32 p_lag_ms = 0)
		{
			// Go back in time though
			const auto tickrate = TICK_RATE.as_seconds();
			const uint32 ms_to_tick = uint32(p_lag_ms / TICK_RATE.as_milliseconds());
			const uint32 used_tick = p_sync_tick - ms_to_tick;

			PositionTimeState* from_ptr = nullptr;
			PositionTimeState* to_ptr = nullptr;

			for(int i = 0; i < 64; i++)
			{
				from_ptr = &m_buffer.previous(i);
				if(to_ptr)
					if(from_ptr->m_tick < used_tick)
						break;
				to_ptr = from_ptr;
			}


			const auto interpolation_full_time = int64(to_ptr->m_tick) - int64(from_ptr->m_tick);
			const auto current_interpolation_time_point = int64(used_tick) - int64(from_ptr->m_tick);
			const float t = float(current_interpolation_time_point) / float(interpolation_full_time);

			m_position = Vector2::lerp(from_ptr->m_state, to_ptr->m_state, t);
		}

	private:
		RingBuffer<PositionTimeState, 64> m_buffer;
		Vector2& m_position;
	};

	struct State : network::DataTransferFunctions
	{
		State() = delete;
		State(const State&) = delete;
		State& operator=(const State&) = delete;

		enum Type 
		{
			ERROR,
			MENU,
			FIND_SERVER,
			LOBBY,
			GAME,
			MENU_TIMEOUT,
		};

		State(ClientApp& p_context, Type p_type);
		

		ClientApp& get_context() { return m_context; }

		void set_type(State::Type p_type) { m_type = p_type; }
		Type get_type() { return m_type; }
		virtual void on_enter() = 0;
		virtual void on_exit() = 0;
		virtual void on_input(const Keyboard::Key& p_key, const Keyboard::KeyState& p_state) = 0;
		virtual void on_tick(const Time& dt) = 0;
		virtual void on_draw() = 0;
		virtual void on_acknowledge(network::Connection* connection, 
							const uint16 sequence) = 0;

	private:
		ClientApp& m_context;
		Type m_type = ERROR;
	};

	struct Menu : State
	{
		enum class MenuTransitions
		{
			FIND_SERVER,
			EXIT
		};

		const static int MENU_POINTS = 2;
		uint8 m_index = 0;
		std::string m_menu_text[MENU_POINTS] =
		{
			"FIND SERVER",
			"CLOSE"
		};


		Menu(ClientApp& p_context);

		// Inherited via GameState
		virtual void on_enter() override;
		virtual void on_exit() override;
		virtual void on_input(const Keyboard::Key& p_key, const Keyboard::KeyState& p_state) override;
		virtual void on_tick(const Time& dt) override;
		virtual void on_draw() override;
		virtual void on_acknowledge(network::Connection* connection, const uint16 sequence) override;
	};

	struct MenuTimeout : Menu
	{
		MenuTimeout(ClientApp& p_context);
		std::string m_timeout_message = "SERVER HAS TIMED OUT :(";
		virtual void on_draw() override;
	};

	struct Game : State
	{
		uint32 m_current_server_tick{ 0 };

		uint8 m_input_bits{ 0 };

		bool m_game_over = false;
		uint32 m_game_end_tick = 0;

		gameplay::Player m_players[4];
		PositionInterpolator m_interpolators[4];
		PostionInputinator m_inputinator;

		std::string game_end_text;
		

		Game(ClientApp& p_context);

		// Inherited via GameState
		virtual void on_enter() override;
		virtual void on_exit() override;
		virtual void on_input(const Keyboard::Key& p_key, const Keyboard::KeyState& p_state) override;
		virtual void on_tick(const Time& dt) override;
		virtual void on_draw() override;
		virtual void on_acknowledge(network::Connection* connection, const uint16 sequence) override;
	};

	struct FindServer : State
	{
		int16 m_index = 0;
		network::ServerDiscoveryConnection m_discovery;

		FindServer(ClientApp& p_context);

		// Inherited via GameState
		virtual void on_enter() override;
		virtual void on_exit() override;
		virtual void on_input(const Keyboard::Key& p_key, const Keyboard::KeyState& p_state) override;
		virtual void on_tick(const Time& dt) override;
		virtual void on_draw() override;
		virtual void on_acknowledge(network::Connection* connection, const uint16 sequence) override;
	};

	struct Lobby : State
	{
		bool m_ready{ false };
		bool m_server_status{ false };

		uint8 m_player_count = 0;
		uint32 m_start_game_tick = 0;
		uint32 m_server_tick;

		Lobby(ClientApp& p_context);

		void receive_server_status(network::Connection* connection, network::NetworkStreamReader& reader);
		void send_client_ready(network::Connection* connection, const uint16 sequence, network::NetworkStreamWriter& writer);

		// Inherited via GameState
		virtual void on_enter() override;
		virtual void on_exit() override;
		virtual void on_input(const Keyboard::Key& p_key, const Keyboard::KeyState& p_state) override;
		virtual void on_tick(const Time& dt) override;
		virtual void on_draw() override;
		virtual void on_acknowledge(network::Connection* connection, const uint16 sequence) override;
	};

	struct StateHandler
	{
		void add_state(State& p_state);
		void remove_state(State& p_state);
		void set_state(State& p_state);

		void on_input(const Keyboard::Key& p_key, const Keyboard::KeyState& p_state);
		void on_tick(const Time& dt);
		void on_draw();
		void on_acknowledge(network::Connection* connection, const uint16 sequence);
		void on_receive(network::Connection* connection, network::NetworkStreamReader& reader);
		void on_send(network::Connection* connection, const uint16 sequence, network::NetworkStreamWriter& writer);

	private:
		State* m_current = nullptr;
		DynamicArray<State*> m_states;
	};

	Menu m_menu;
	MenuTimeout m_menu_timeout;
	FindServer m_find_server;
	Lobby m_lobby;
	Game m_game;
	
	StateHandler m_state_handler;

	int m_sequence = 0;
	gameplay::Tilemap m_tilemap;
	gameplay::Grid m_grid;

   const static Time TICK_RATE;


   struct InputinatorState
   {
	   InputinatorState()
		   : m_input(0)
		   , m_tick(0)
		   , m_position(0, 0)
	   {

	   }

	   uint8 m_input;
	   Vector2 m_position;
	   uint32 m_tick;
   };
   struct Inputinator
   {
	   Inputinator()
	   {

	   }

	   void push(const InputinatorState& p_state)
	   {
		   m_buffer.push(p_state);
	   }

	   RingBuffer<InputinatorState, 16> m_buffer;
   };


   // note: Application
   virtual bool on_init();
   virtual void on_exit();
   virtual bool on_tick(const Time &dt);
   virtual void on_draw();

   // note: IConnectionListener 
   virtual void on_acknowledge(network::Connection *connection, const uint16 sequence);
   virtual void on_receive(network::Connection *connection, network::NetworkStreamReader &reader);
   virtual void on_send(network::Connection *connection, const uint16 sequence, network::NetworkStreamWriter &writer);
	  
   Mouse &m_mouse;
   Keyboard &m_keyboard;
   network::Connection m_server_connection;

   uint8 m_id_on_server;
   uint32 m_ticks;
   RingBuffer<TimeState, 64> m_server_times;

   Time m_accumulator;
   

   bool m_exit;
   //TimeStatesBuffer m_time_states;
   //RingBuffer<uint8, 16> m_inputs;

   // NETWORK OVERLAY
   uint64 dropped_packets = 0;
   bool m_draw_network_overlay = true;
   DynamicArray<std::pair<Time, float>> m_pair_buffer;
   Time m_one_second_timer;
   enum
   {
	   PREVIOUS,
	   CURRENT,
	   COUNT
   };
   NetworkInformation::Overlay m_network_overlay;
   float m_network_data[COUNT][NetworkInformation::NETWORK_DATA_TYPE_COUNT] = { { 0.0f }, { 0.0f } };
   std::string m_network_overlay_titles[NetworkInformation::NETWORK_DATA_TYPE_COUNT] =
   {
			"Total Kibibytes sent: ",
			"Total Kibibytes received: ",
			"Kibitypes/s sent: ",
			"Kibitypes/s received: ",
			"Total Packets sent: ",
			"Total Packets received: ",
			"Packert/s sent: ",
			"Packets/s received: ",
			"Packet loss: ",
			"Current RTT (ms): ",
			"Lower RTT (ms): ",
			"Highest RTT (ms): ",
			"Average RTT (ms): ",
			"Total Messages sent: ",
			"Total Messages received: ",
			"Misspredicted Inputs: "
   };
   // !NETWORK OVERLAY
};

#endif // !CLIENT_APP_HPP_INCLUDED
