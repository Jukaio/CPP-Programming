// client_app.hpp

#ifndef CLIENT_APP_HPP_INCLUDED
#define CLIENT_APP_HPP_INCLUDED

#include <charlie_application.hpp>

using namespace charlie;

struct ClientApp final : Application, network::IConnectionListener {
   ClientApp();

   constexpr static size_t SERVER_TIME_STATE = 32;

   struct EntityState {
	   EntityState()
		   : m_tick(0)
		   , m_time(0.0)
		   , m_position(0.0f, 0.0f)
	   {

	   }
	   uint32 m_tick;		// order and identificator
	   Time m_time;			// if m dt == 1 -> drop this state
	   Vector2 m_position; // the position
   };
   struct StateBuffer
   {
	   void add(const EntityState& p_state)
	   {
		   m_buffer.push_back(p_state);
	   }

	   EntityState pop_front()
	   {
		   EntityState to_return = m_buffer.front();
		   m_buffer.erase(m_buffer.begin());
		   return to_return;
	   }

	   void remove(EntityState p_state)
	   {
		   auto it = m_buffer.begin();
		   while (it != m_buffer.end())
		   {
			   if ((*it).m_tick == p_state.m_tick)
			   {
				   m_buffer.erase(it);
			   }
		   }
	   }

	   DynamicArray<EntityState> m_buffer;
   };

   struct Interpolator
   {
	   Interpolator(Time p_lag)
		   : m_index(0)
		   , m_current(0)
		   , m_next(1)
		   , m_prev_index(BUFFER_SIZE - 1)
		   , m_last_client_step(Time::now())
		   , m_client_lag(p_lag)
	   {

	   }

	   void push(const EntityState& p_state)
	   {
		   EntityState state;
		   state.m_position = p_state.m_position;
		   state.m_tick = p_state.m_tick;
		   state.m_time = p_state.m_time;

		   EntityState previous = m_buffer[m_prev_index];
		   m_buffer[m_index] = state;

		   m_prev_index = m_index;
		   m_index = (m_index + 1) % BUFFER_SIZE;
	   }

	   EntityState step(Time p_client_now)
	   {
		   const Time difference = m_last_client_step - p_client_now;
		   const Time client_current = p_client_now - m_client_lag;

		   if (client_current < Time(0.0))
			   return m_buffer[0];

		   EntityState prev = m_buffer[BUFFER_SIZE - 1];
		   EntityState current;
		   for (int i = 0; i < BUFFER_SIZE; i++)
		   {
			   current = m_buffer[i];
			   if (client_current >= prev.m_time &&
				   client_current < current.m_time)
				   break;
			   prev = current;
		   }

		   const EntityState& from = prev;
		   const EntityState& to = current;

		   const Time interpolation_full = to.m_time - from.m_time;
		   const Time current_time_point = client_current - from.m_time;
		   const float t = current_time_point.as_seconds() / interpolation_full.as_seconds();
		   if (t == INFINITY)
			   printf("ERROR\n"); // TODO: Check timers, something is off with them
		   printf("t: %f --", t);
		   EntityState to_return;
		   to_return.m_tick = from.m_tick;
		   to_return.m_time = to.m_time;
		   to_return.m_position = Vector2::lerp(from.m_position, to.m_position, t);

		   m_last_client_step = p_client_now;

		   return to_return;
	   }

	   int m_current;
	   int m_next;
	   int m_index;
	   int m_prev_index;

	   Time m_last_client_step;
	   Time m_client_lag;

	   constexpr static int BUFFER_SIZE = 128;
	   EntityState m_buffer[BUFFER_SIZE];
   };
   struct TickInterpolator
   {
	   void push(const EntityState& p_state)
	   {
		   if (!m_buffer.m_buffer.empty())
		   {
			   EntityState previous = m_buffer.m_buffer.back();
			   const int range_count = (p_state.m_tick - 1) - (previous.m_tick + 1);
			   const float step = 1.0f / range_count;
			   float t = 0;
			   for (uint32 i = (previous.m_tick + 1); i < p_state.m_tick; i++)
			   {
				   t = t + step;

				   EntityState temp;
				   temp.m_tick = i;
				   temp.m_time = p_state.m_time;
				   temp.m_position = Vector2::lerp(previous.m_position, p_state.m_position, t);
				   m_buffer.add(temp);
			   }
		   }
		   m_buffer.add(p_state);
	   }

	   bool empty()
	   {
		   return m_buffer.m_buffer.empty();
	   }

	   EntityState step(int p_back_in_time)
	   {
		   if (m_buffer.m_buffer.size() > p_back_in_time)
			   return m_buffer.pop_front();
		   return m_buffer.m_buffer.front();
	   }

	   StateBuffer m_buffer;
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

   Mouse &mouse_;
   Keyboard &keyboard_;
   network::Connection connection_;

   uint32 ticks_;
   Time accumulator_;

   gameplay::Entity entity_;
   Interpolator m_interpolator;

   Buffer<TimeState> m_time_states;
};

#endif // !CLIENT_APP_HPP_INCLUDED
