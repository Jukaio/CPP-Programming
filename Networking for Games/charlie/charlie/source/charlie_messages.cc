// charlie_messages.cc

#include "charlie_messages.hpp"
#include "charlie_network.hpp"

namespace charlie {
   namespace network {
      NetworkMessageServerTick::NetworkMessageServerTick()
         : type_(NETWORK_MESSAGE_SERVER_TICK)
         , server_time_(0)
         , server_tick_(0)
      {
      }

      NetworkMessageServerTick::NetworkMessageServerTick(const int64  server_time,
                                                         const uint32 server_tick)
         : type_(NETWORK_MESSAGE_SERVER_TICK)
         , server_time_(server_time)
         , server_tick_(server_tick)
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
	  {
	  }
	  NetworkMessageClientTick::NetworkMessageClientTick(const int64 client_time, const uint32 client_tick)
		  : type_(NETWORK_MESSAGE_CLIENT_TICK)
		  , client_time_(client_time)
		  , client_tick_(client_tick)
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
	  NetworkMessageInputCommand::NetworkMessageInputCommand()
		  : type_(NETWORK_MESSAGE_INPUT_COMMAND)
		  , bits_(0)
	  {
	  }
	  NetworkMessageInputCommand::NetworkMessageInputCommand(const uint8& bits)
		  : type_(NETWORK_MESSAGE_INPUT_COMMAND)
		  , bits_(bits)
	  {
	  }
	  bool NetworkMessageInputCommand::read(NetworkStreamReader& reader)
	  {
		  return serialize(reader);
	  }
	  bool NetworkMessageInputCommand::write(NetworkStreamWriter& writer)
	  {
		  return serialize(writer);
	  }
	  NetworkMessagePlayerState::NetworkMessagePlayerState()
		  : type_(NETWORK_MESSAGE_PLAYER_STATE)

	  {
	  }
	  NetworkMessagePlayerState::NetworkMessagePlayerState(const Vector2& position)
		  : type_(NETWORK_MESSAGE_PLAYER_STATE)
		  , position_(position)

	  {
	  }
	  bool NetworkMessagePlayerState::read(NetworkStreamReader& reader)
	  {
		  return serialize(reader);
	  }
	  bool NetworkMessagePlayerState::write(NetworkStreamWriter& writer)
	  {
		  return serialize(writer);
	  }
   } // !network
} // !messages
