// Interpolator.h

#ifndef INCLUDED_INTERPOLATOR_H
#define INCLUDED_INTERPOLATOR_H

#include <charlie.hpp>
#include <charlie_gameplay.hpp>

//template<typename StateData>
//struct InterpolationState
//{
//	StateData m_state;
//	charlie::int64 m_tick{ 0 };
//};
//
////template<typename StateData, const int buffer_size>
//struct Interpolator
//{
//	Interpolator()
//	{
//
//	}
//
//	void push(const int& p_state, charlie::int64 p_tick)
//	{
//		InterpolationState<int> state;
//		state.m_state = p_state;
//		state.m_tick = p_tick;
//
//		m_buffer.push(state);
//	}
//
//	int step(charlie::int64 m_ticks, charlie::int64& p_last_tick, charlie::int64& p_previous_tick)
//	{
//		const auto& from = m_buffer.previous();
//		const auto& to = m_buffer.current();
//
//		const auto difference = p_last_time_state.m_server.ticks - p_last_time_state.m_client.ticks;
//		const auto one_step_back = to.m_tick - from.m_tick;
//		const auto synced_client_current = m_ticks - one_step_back + difference;
//
//		/*
//		const Time difference = p_last_time_state.m_server.now - p_last_time_state.m_client.now;
//		const Time one_step_back = p_last_time_state.m_server.now - p_previous_time_state.m_server.now;
//		const Time synced_client_current = p_client_now - one_step_back + difference;*/
//
//		const auto interpolation_full_time = to.m_tick - from.m_tick;
//		const auto current_interpolation_time_point = static_cast<int64>(synced_client_current) - static_cast<int64>(from.m_tick);
//		const float t = static_cast<float>(current_interpolation_time_point) / static_cast<float>(interpolation_full_time);
//
//		EntityState to_return;
//		to_return.m_tick = from.m_tick;
//		to_return.m_position = Vector2::lerp(from.m_position, to.m_position, t);
//
//		return to_return;
//	}
//
//	charlie::RingBuffer<InterpolationState<int>, 16> m_buffer;
//};

#endif // !INCLUDED_INTERPOLATOR_H
