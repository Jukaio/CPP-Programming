// charlie_messages.cc

#include "charlie_messages.hpp"
#include "charlie_network.hpp"

namespace charlie {
   namespace network {
      NetworkMessageServerTick::NetworkMessageServerTick()
         : type_(NETWORK_MESSAGE_SERVER_TICK)
         , server_time_(0)
         , server_tick_(0)
		 , server_dt_(0)
      {
      }

      NetworkMessageServerTick::NetworkMessageServerTick(const int64  server_time,
                                                         const uint32 server_tick,
														 const int64	server_dt)
         : type_(NETWORK_MESSAGE_SERVER_TICK)
         , server_time_(server_time)
         , server_tick_(server_tick)
		 , server_dt_(server_dt)
      {
      }

      bool NetworkMessageServerTick::read(NetworkStreamReader &reader)
      {
         return serialize(reader);
      }

      bool NetworkMessageServerTick::write(NetworkStreamWriter &writer)
      {
         return serialize(writer);
      }

      NetworkMessageEntityState::NetworkMessageEntityState()
         : type_(NETWORK_MESSAGE_ENTITY_STATE)
      {
      }

      NetworkMessageEntityState::NetworkMessageEntityState(const Vector2 &position)
         : type_(NETWORK_MESSAGE_ENTITY_STATE)
         , position_(position)
      {
      }

      bool NetworkMessageEntityState::read(NetworkStreamReader &reader)
      {
         return serialize(reader);
      }

      bool NetworkMessageEntityState::write(NetworkStreamWriter &writer)
      {
         return serialize(writer);
      }

	  NetworkMessageClientTick::NetworkMessageClientTick()
		  : type_(NETWORK_MESSAGE_CLIENT_TICK)
		  , client_time_(0)
		  , client_tick_(0)
		  , client_dt_(0)
	  {
	  }
	  NetworkMessageClientTick::NetworkMessageClientTick(const int64 client_time, const uint32 client_tick, const int64 client_dt)
		  : type_(NETWORK_MESSAGE_CLIENT_TICK)
		  , client_time_(client_time)
		  , client_tick_(client_tick)
		  , client_dt_(client_dt)
	  {
	  }
	  bool NetworkMessageClientTick::read(NetworkStreamReader& reader)
	  {
		  return serialize(reader);
	  }
	  bool NetworkMessageClientTick::write(NetworkStreamWriter& writer)
	  {
		  return serialize(writer);
	  }
   } // !network
} // !messages
