// charlie_messages.hpp

#ifndef CHARLIE_MESSAGES_HPP_INCLUDED
#define CHARLIE_MESSAGES_HPP_INCLUDED

#include <charlie.hpp>
#include <string>
#include <typeinfo>

/*
    Significant bandwith usage cutdowns:
    Player Replication:
    instead of Vector2 -> uint16 for each player:
    8 byte -> 4 byte = cutdown per player is about 50%

    other very significant improvement, 
    I wanted to download a map first, but I managed to make the maps so small
    that I was able to send it as the whole world state:

    map size = 16 * 16 -> 256 bytes
    I managed to get it down to 64 bytes.
    uint32 represents a whole row of the tilemap,
    each 2 bits in the uint32 row represents a tile in the row
    */

namespace charlie 
{
   namespace network 
   {
      struct NetworkStream;
      struct NetworkStreamReader;
      struct NetworkStreamWriter;
      
      namespace message
      {

          enum Type 
          {
             NETWORK_MESSAGE_TICK,
		     NETWORK_MESSAGE_INPUT_COMMAND,
             NETWORK_MESSAGE_MAP,
             NETWORK_MESSAGE_CLIENT_READY,
             NETWORK_MESSAGE_SERVER_STATUS,
             NETWORK_MESSAGE_PLAYER_REPLICATIONS,
             NETWORK_MESSAGE_GAME_STATUS,
             NETWORK_MESSAGE_BOMB,

             NETWORK_MESSAGE_COUNT,
             NETWORK_MESSAGE_CONTENT_INFO,
          };

          /*
          enum Flag
          {
              CONTENT_FLAG_TICK             = 1 << NETWORK_MESSAGE_TICK,
              CONTENT_FLAG_INPUT_COMMAND    = 1 << NETWORK_MESSAGE_INPUT_COMMAND,
              CONTENT_FLAG_MAP              = 1 << NETWORK_MESSAGE_MAP,
              CONTENT_FLAG_CLIENT_READY     = 1 << NETWORK_MESSAGE_CLIENT_READY,
              CONTENT_FLAG_SERVER_STATUS    = 1 << NETWORK_MESSAGE_SERVER_STATUS,

              CONTENT_FLAG_COUNT            = 1 << NETWORK_MESSAGE_COUNT
          };*/
          
          static_assert(NETWORK_MESSAGE_COUNT <= 255, "network message type cannot exceed 255!");
           
          struct ContentInfo
          {
              ContentInfo();
              explicit ContentInfo(uint16 p_content_flag);

              static void set_flag(uint8* p_at, uint16 p_set);

              bool read(NetworkStreamReader& reader);
              bool write(NetworkStreamWriter& writer);

              template <typename Stream>
              bool serialize(Stream& stream)
              {
                  bool result = true;
                  result &= stream.serialize(type_);
                  result &= stream.serialize(m_content_flag);
                  return result;
              }

              uint8 type_;
              uint16 m_content_flag; // 
          };

          struct Tick 
          {
             Tick();
             explicit Tick(const uint32 p_tick);

             bool read(NetworkStreamReader &reader);
             bool write(NetworkStreamWriter &writer);

             template <typename Stream>
             bool serialize(Stream &stream)
             {
                bool result = true;
                result &= stream.serialize(type_);
                result &= stream.serialize(tick_);
                return result;
             }

             uint8 type_;
             uint32 tick_;
          };

	      struct InputCommand {
		      InputCommand();
		      explicit InputCommand(const uint8& bits, const uint8& p_pos_x, const uint8& p_pos_y, const uint32& p_tick);

		      bool read(NetworkStreamReader& reader);
		      bool write(NetworkStreamWriter& writer);

		      template <typename Stream>
		      bool serialize(Stream& stream)
		      {
			      bool result = true;
			      result &= stream.serialize(type_);
			      result &= stream.serialize(bits_);
                  result &= stream.serialize(pos_x_);
                  result &= stream.serialize(pos_y_);
                  result &= stream.serialize(tick_);
			      return result;
		      }

		      uint8 type_;
		      uint8 bits_;
              uint8 pos_x_;
              uint8 pos_y_;
              uint32 tick_;
	      };

          struct MapData
          {
              MapData();
              explicit MapData(uint32(&p_transfer_buffer)[16]);

              bool read(NetworkStreamReader& reader);
              bool write(NetworkStreamWriter& writer);

              template <typename Stream>
              bool serialize(Stream& stream)
              {
                  bool result = true;
                  result &= stream.serialize(type_);
                  for(int i = 0; i < 16; i++)
                      result &= stream.serialize(map_data_[i]);
                  return result;
              }


              uint8 type_;
              uint32 map_data_[16] = { 0 };
          };

          struct ClientReady
          {
              ClientReady();
              explicit ClientReady(uint8 p_ready);

              bool read(NetworkStreamReader& reader);
              bool write(NetworkStreamWriter& writer);

              template <typename Stream>
              bool serialize(Stream& stream)
              {
                  bool result = true;
                  result &= stream.serialize(type_);
                  result &= stream.serialize(ready_);
                  return result;
              }

              uint8 type_;
              uint8 ready_;
          };

          struct ServerStatus
          {
              ServerStatus();
              explicit ServerStatus(uint8 p_ready, uint8 p_player_count, const uint8 p_id, uint32 p_game_start_timer);

              bool read(NetworkStreamReader& reader);
              bool write(NetworkStreamWriter& writer);

              template <typename Stream>
              bool serialize(Stream& stream)
              {
                  bool result = true;
                  result &= stream.serialize(type_);
                  result &= stream.serialize(ready_);
                  result &= stream.serialize(player_count_);
                  result &= stream.serialize(client_id_);
                  result &= stream.serialize(game_start_tick_);
                  return result;
              }

              uint8 type_;
              uint8 ready_;
              uint8 player_count_;
              uint8 client_id_;
              uint32 game_start_tick_;
          };

          struct GameStatus
          {
              GameStatus();
              explicit GameStatus(uint8 p_game_over, uint32 p_game_end_tick);

              bool read(NetworkStreamReader& reader);
              bool write(NetworkStreamWriter& writer);

              template <typename Stream>
              bool serialize(Stream& stream)
              {
                  bool result = true;
                  result &= stream.serialize(type_);
                  result &= stream.serialize(game_over_);
                  result &= stream.serialize(game_end_tick_);
                  return result;
              }

              uint8 type_;
              uint8 game_over_;
              uint32 game_end_tick_;
          };

          struct ReplicatePlayers
          {
              ReplicatePlayers();
              explicit ReplicatePlayers(uint8 player_active_mask_, uint64 p_player_positions, uint32 p_tick);

              bool read(NetworkStreamReader& reader);
              bool write(NetworkStreamWriter& writer);

              template <typename Stream>
              bool serialize(Stream& stream)
              {
                  bool result = true;
                  result &= stream.serialize(type_);
                  result &= stream.serialize(player_active_mask_);
                  result &= stream.serialize(replica_positions_);
                  result &= stream.serialize(tick_);
                  return result;
              }

              uint8 type_;
              uint8 player_active_mask_; // 4 bits for client active other 4 for unit active
              uint64 replica_positions_;
              uint32 tick_;
          };

          struct Bomb
          {
              Bomb();
              explicit Bomb(const uint8 p_x, const uint8 p_y, uint32 p_target_tick, const uint8 p_planter_id);

              bool read(NetworkStreamReader& reader);
              bool write(NetworkStreamWriter& writer);

              template <typename Stream>
              bool serialize(Stream& stream)
              {
                  bool result = true;
                  result &= stream.serialize(type_);
                  result &= stream.serialize(x_);
                  result &= stream.serialize(y_);
                  result &= stream.serialize(target_tick_);
                  result &= stream.serialize(planter_id_);
                  return result;
              }

              uint8 type_;
              uint8 x_;
              uint8 y_;
              uint32 target_tick_;
              uint8 planter_id_;
          };

          struct ReliableMessage
          {
              uint8 type_{ (uint8)~0 };
              DynamicArray<uint32> acks_;
          };

          struct ReliableBomb : ReliableMessage
          {
              ReliableBomb()
              {
                  type_ = NETWORK_MESSAGE_BOMB;
              }
              Bomb m_bomb;
          };
      }
      // !message
   } // !network
} // !charlie

#endif // !CHARLIE_MESSAGES_HPP_INCLUDED
