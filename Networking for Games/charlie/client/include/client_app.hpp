// client_app.hpp

#ifndef CLIENT_APP_HPP_INCLUDED
#define CLIENT_APP_HPP_INCLUDED

#include <charlie_application.hpp>

using namespace charlie;

typedef RingBuffer<TimeState, 64> TimeStatesBuffer;

struct ClientApp final : Application, network::IConnectionListener {
   ClientApp();

   const static Time TICK_RATE;

   struct EntityState {
	   EntityState()
		   : m_tick(0)
		   , m_position(0.0f, 0.0f)
	   {

	   }
	   uint32 m_tick;		// order and identificator
	   Vector2 m_position;  // the position
   };

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

   struct Interpolator
   {
	   Interpolator()
	   {

	   }

	   void push(const EntityState& p_state)
	   {
		   EntityState state;
		   state.m_position = p_state.m_position;
		   state.m_tick = p_state.m_tick;

		   m_buffer.push(state);
	   }

	   EntityState step(uint32 p_sync_tick)
	   {
		   // Go back in time though
		   const EntityState& from = m_buffer.previous();
		   const EntityState& to = m_buffer.current();

		   const auto interpolation_full_time = int64(to.m_tick) - int64(from.m_tick);
		   const auto current_interpolation_time_point = int64(p_sync_tick) - int64(from.m_tick);
		   const float t = float(current_interpolation_time_point) / float(interpolation_full_time);
		   
		   EntityState to_return;
		   to_return.m_tick = from.m_tick;
		   to_return.m_position = Vector2::lerp(from.m_position, to.m_position, t);

		   return to_return;
	   }

	   RingBuffer<EntityState, 16> m_buffer;
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
  

   struct EntityClient
   {
	   EntityClient() : m_context() {}
	   void set_context(gameplay::Entity* p_context)
	   {
		   m_context = p_context;
	   }

	   void update(uint32 ticks)
	   {
		   const EntityState state = m_interpolator.step(ticks);
		   m_context->transform_.set_position(state.m_position);
	   }

	   void render(Renderer p_renderer)
	   {
		   
		   p_renderer.render_rectangle_fill(  { int32(m_context->transform_.position_.x_), 
										        int32(m_context->transform_.position_.y_),
											    20, 
											    20 }, 
											  Color::Green);
	   }

	   void push_entity_state(EntityState& p_state)
	   {
		   m_interpolator.push(p_state);
	   }

	   Interpolator m_interpolator;
	   gameplay::Entity* m_context;
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
   int64 m_synced_ticks;
   Time accumulator_;

   gameplay::Entity m_context;
   EntityClient m_entity;

   TimeStatesBuffer m_time_states;

   gameplay::Player m_player;
   RingBuffer<uint8, 16> m_inputs;
};

#endif // !CLIENT_APP_HPP_INCLUDED
