// charlie_messages.hpp

#ifndef CHARLIE_MESSAGES_HPP_INCLUDED
#define CHARLIE_MESSAGES_HPP_INCLUDED

#include <charlie.hpp>

namespace charlie {
   namespace network {
      struct NetworkStreamReader;
      struct NetworkStreamWriter;

      enum NetworkMessageType {
         NETWORK_MESSAGE_SERVER_TICK,
		 NETWORK_MESSAGE_CLIENT_TICK,
         NETWORK_MESSAGE_ENTITY_STATE,
         NETWORK_MESSAGE_COUNT,
      };

      static_assert(NETWORK_MESSAGE_COUNT <= 255, "network message type cannot exceed 255!");


	  struct NetworkMessageClientTick {
		  NetworkMessageClientTick();
		  explicit NetworkMessageClientTick(const int64  client_time,
											const uint32 client_tick,
											const int64	client_dt);

		  bool read(NetworkStreamReader& reader);
		  bool write(NetworkStreamWriter& writer);

		  template <typename Stream>
		  bool serialize(Stream& stream)
		  {
			  bool result = true;
			  result &= stream.serialize(type_);
			  result &= stream.serialize(client_time_);
			  result &= stream.serialize(client_tick_);
			  result &= stream.serialize(client_dt_);
			  return result;
		  }

		  uint8 type_;
		  int64 client_time_;
		  uint32 client_tick_;
		  int64 client_dt_;
	  };

      struct NetworkMessageServerTick {
         NetworkMessageServerTick();
         explicit NetworkMessageServerTick(const int64  server_time,
                                           const uint32 server_tick,
										   const int64	server_dt);

         bool read(NetworkStreamReader &reader);
         bool write(NetworkStreamWriter &writer);

         template <typename Stream>
         bool serialize(Stream &stream)
         {
            bool result = true;
            result &= stream.serialize(type_);
            result &= stream.serialize(server_time_);
            result &= stream.serialize(server_tick_);
			result &= stream.serialize(server_dt_);
            return result;
         }

         uint8 type_;
         int64 server_time_;
         uint32 server_tick_;
		 int64 server_dt_;
      };

      struct NetworkMessageEntityState {
         NetworkMessageEntityState();
         explicit NetworkMessageEntityState(const Vector2 &position);

         bool read(NetworkStreamReader &reader);
         bool write(NetworkStreamWriter &writer);

         template <typename Stream>
         bool serialize(Stream &stream)
         {
            bool result = true;
            result &= stream.serialize(type_);
            result &= stream.serialize(position_.x_);
            result &= stream.serialize(position_.y_);
            return result;
         }

         uint8 type_;
         Vector2 position_;
      };

   } // !network
} // !charlie

#endif // !CHARLIE_MESSAGES_HPP_INCLUDED
