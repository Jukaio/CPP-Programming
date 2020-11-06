// server_app.hpp

#ifndef SERVER_APP_HPP_INCLUDED
#define SERVER_APP_HPP_INCLUDED

#include <charlie_application.hpp>
#include <network_information.h>
#include <charlie_messages.hpp>

using namespace charlie;

struct Client
{
	int32 m_id{ -1 };
	uint64 m_connection{ 0 };
	RingBuffer<TimeState, 64> m_time_state;
};
struct ClientList
{
	ClientList()
	{
		for(int i = 0; i < 10; i++)
			m_id_pool.push_back(i);
	}

	int pop_front_id()
	{
		int id = m_id_pool.front();
		m_id_pool.erase(m_id_pool.begin());
		return id;
	}

	void push_front_id(int p_id)
	{
		for(auto& id : m_id_pool)
		{
			if(id == p_id)
				return;
		}
		m_id_pool.insert(m_id_pool.begin(), p_id);
	}

	int64 add_client(const uint64 connection)
	{
		const int32 id = pop_front_id();
		m_clients.push_back({ id, connection });
		return id;
	}

	void remove_client(const uint64 connection)
	{
		auto it = m_clients.begin();
		while (it != m_clients.end()) 
		{
			if ((*it).m_connection == connection) 
			{
				push_front_id((*it).m_id);
				m_clients.erase(it);
				break;
			}
			it++;
		}
	}

	Client* find_client(const uint64 p_connection)
	{
		for (auto& client : m_clients)
		{
			if (client.m_connection == p_connection)
				return &client;
		}
		return nullptr;
	}

	DynamicArray<Client> m_clients;
	DynamicArray<int32> m_id_pool;
};

struct ServerApp final : Application, network::IServiceListener, network::IConnectionListener 
{
   static const int CLIENT_LIMIT = 12;
   static const int MINIMUM_CLIENTS_TO_START = 2;

   ServerApp();
   ServerApp(const ServerApp&) = delete;
   ServerApp& operator=(const ServerApp&) = delete;

   struct State : network::DataTransferFunctions
   {
	   enum Type
	   {
		   ERROR,	
		   WAITING,
		   PLAYING
	   };

	   State(ServerApp& p_context, Type p_type);

	   ServerApp& get_context() { return m_context; }

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
	   ServerApp& m_context;
	   Type m_type = ERROR;
   };

   struct Waiting : State
   {
	   enum PlayerState
	   {
		   NOT_READY,
		   READY,
	   };

	   bool m_game_starts =  false;
	   uint32 m_game_start_tick;

	   PlayerState m_player_states[CLIENT_LIMIT] = { NOT_READY };
	   DynamicArray<uint16> m_reliable_queue;

	   Waiting(ServerApp& p_context);

	   bool ready_to_play();

	   // Inherited via State
	   virtual void on_enter() override;
	   virtual void on_exit() override;
	   virtual void on_input(const Keyboard::Key& p_key, const Keyboard::KeyState& p_state) override;
	   virtual void on_tick(const Time& dt) override;
	   virtual void on_draw() override;
	   virtual void on_acknowledge(network::Connection* connection,
								   const uint16 sequence) override;
   };

   struct Playing : State
   {
	   bool m_game_ends = false;
	   uint32 m_game_end_ticks;
	   const static int MAX_PLAYERS = 4;
	   gameplay::Player m_players[MAX_PLAYERS] = { false, Transform{} };
	   RingBuffer<std::pair<uint32, Vector2>, 64> m_position_tick_buffer[MAX_PLAYERS];

	   gameplay::Tilemap m_tilemap;

	   network::message::ReliableBomb m_reliable_bomb_bool[MAX_PLAYERS];
	   DynamicArray<network::message::ReliableMessage*> m_reliable_message_buffer[MAX_PLAYERS];
	   // ack, bomb
	   /*
	   DynamicArray<std::pair<network::message::Bomb, DynamicArray<uint16>>> m_reliable_bombs[MAX_PLAYERS] = 
	   {
		   DynamicArray<std::pair<network::message::Bomb, DynamicArray<uint16>>>{ 64, { {}, DynamicArray<uint16>(128)}},
		   DynamicArray<std::pair<network::message::Bomb, DynamicArray<uint16>>>{ 64, { {}, DynamicArray<uint16>(128)}},
		   DynamicArray<std::pair<network::message::Bomb, DynamicArray<uint16>>>{ 64, { {}, DynamicArray<uint16>(128)}},
		   DynamicArray<std::pair<network::message::Bomb, DynamicArray<uint16>>>{ 64, { {}, DynamicArray<uint16>(128)}}
	   };*/

	   //RingBuffer<std::tuple<bool, network::message::Bomb, StaticArray<int32, 64>>, 64> m_reliable_bombs[MAX_PLAYERS];

	   //Queue<uint8> m_inputs[MAX_PLAYERS];

	   uint8 m_inputs[MAX_PLAYERS] = { 0 };

	   Playing(ServerApp& p_context);

	   // Inherited via State
	   virtual void on_enter() override;
	   virtual void on_exit() override;
	   virtual void on_input(const Keyboard::Key& p_key, const Keyboard::KeyState& p_state) override;
	   virtual void on_tick(const Time& dt) override;
	   virtual void on_draw() override;
	   virtual void on_acknowledge(network::Connection* connection,
								   const uint16 sequence) override;
   };

   struct StateHandler
   {
	   void add_state(State& p_state)
	   {
		   for(int i = 0; i < m_states.size(); i++)
			   assert((m_states[i]->get_type() != p_state.get_type()) && "State already exists in state list");
		   m_states.push_back(&p_state);
	   }
	   void remove_state(State& p_state)
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
	   void set_state(State& p_state)
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

	   void on_input(const Keyboard::Key& p_key, const Keyboard::KeyState& p_state)
	   {
		   if(m_current) m_current->on_input(p_key, p_state);
	   }
	   void on_tick(const Time& dt)
	   {
		   if(m_current) m_current->on_tick(dt);
	   }
	   void on_draw()
	   {
		   if(m_current) m_current->on_draw();

	   }
	   void on_acknowledge(network::Connection* connection, const uint16 sequence)
	   {
		   if(m_current) m_current->on_acknowledge(connection, sequence);
	   }
	   void on_receive(network::Connection* connection, network::NetworkStreamReader& reader)
	   {
		   if(m_current) m_current->receive_messages(connection, reader);
	   }
	   void on_send(network::Connection* connection, const uint16 sequence, network::NetworkStreamWriter& writer)
	   {
		   if(m_current) m_current->send_messages(connection, sequence, writer);
	   }

   private:
	   State* m_current = nullptr;
	   DynamicArray<State*> m_states;
   };

   const static Time TICK_RATE;

   DynamicArray<gameplay::Tilemap> m_level_collection;

   gameplay::Grid m_grid;

   // note: Application
   virtual bool on_init();
   virtual void on_exit();
   virtual bool on_tick(const Time &dt);
   virtual void on_draw();

   // note: IServiceListener
   virtual void on_timeout(network::Connection *connection);
   virtual void on_connect(network::Connection *connection);
   virtual void on_disconnect(network::Connection *connection);

   // note: IConnectionListener 
   virtual void on_acknowledge(network::Connection *connection, const uint16 sequence);
   virtual void on_receive(network::Connection *connection, network::NetworkStreamReader &reader);
   virtual void on_send(network::Connection *connection, const uint16 sequence, network::NetworkStreamWriter &writer);

   Time m_accumulator;
   uint32 m_ticks;

   Waiting m_waiting;
   Playing m_playing;

   StateHandler m_state_handler;

   float one_sec = 0.0f;
   int tracker = 0;
   int current_tracker = 0;
   int prev_tracker = 0;

   ClientList m_client_list;
   //uint8 m_player_input;
};

#endif // !SERVER_APP_HPP_INCLUDED
