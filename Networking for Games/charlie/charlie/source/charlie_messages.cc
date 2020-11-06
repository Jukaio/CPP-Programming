// charlie_messages.cc

#include "charlie_messages.hpp"
#include "charlie_network.hpp"

namespace charlie 
{
   namespace network 
   {
      namespace message 
      {
          ContentInfo::ContentInfo()
              : type_(NETWORK_MESSAGE_CONTENT_INFO)
              , m_content_flag(0)
          {
          }

          ContentInfo::ContentInfo(uint16 p_content_flag)
              : type_(NETWORK_MESSAGE_CONTENT_INFO)
              , m_content_flag(p_content_flag)
          {
          }

          void ContentInfo::set_flag(uint8* p_at, uint16 p_set)
          {
              if(p_at[0] == NETWORK_MESSAGE_CONTENT_INFO)
              {
                  p_at[1] = (p_set >> 8) & 0xff;
                  p_at[2] = (p_set) & 0xff;
              }
          }

          bool ContentInfo::read(NetworkStreamReader& reader)
          {
              return serialize(reader);
          }
          bool ContentInfo::write(NetworkStreamWriter& writer)
          {
              return serialize(writer);
          }

          Tick::Tick()
             : type_(NETWORK_MESSAGE_TICK)
             , tick_(0)
          {
          }

          Tick::Tick(const uint32 p_tick)
             : type_(NETWORK_MESSAGE_TICK)
             , tick_(p_tick)
          {
          }

          bool Tick::read(NetworkStreamReader &reader)
          {
             return serialize(reader);
          }

          bool Tick::write(NetworkStreamWriter &writer)
          {
             return serialize(writer);
          }

	      InputCommand::InputCommand()
		      : type_(NETWORK_MESSAGE_INPUT_COMMAND)
		      , bits_(0)
              , pos_x_(0)
              , pos_y_(0)
              , tick_(0)
	      {
	      }
	      InputCommand::InputCommand(const uint8& bits, const uint8& p_pos_x, const uint8& p_pos_y, const uint32& p_tick)
		      : type_(NETWORK_MESSAGE_INPUT_COMMAND)
		      , bits_(bits)
              , pos_x_(p_pos_x)
              , pos_y_(p_pos_y)
              , tick_(p_tick)
	      {
	      }
	      bool InputCommand::read(NetworkStreamReader& reader)
	      {
		      return serialize(reader);
	      }
	      bool InputCommand::write(NetworkStreamWriter& writer)
	      {
		      return serialize(writer);
	      }

          MapData::MapData()
            : type_(NETWORK_MESSAGE_MAP)
          {
          }
          MapData::MapData(uint32 (&p_transfer_buffer)[16])
              : type_(NETWORK_MESSAGE_MAP)
          {     

                memcpy(map_data_, p_transfer_buffer, sizeof(map_data_));
          }

          bool MapData::read(NetworkStreamReader& reader)
          {
              return serialize(reader);
          }
          bool MapData::write(NetworkStreamWriter& writer)
          {

              return serialize(writer);
          }

          ClientReady::ClientReady()
            : type_(NETWORK_MESSAGE_CLIENT_READY)
            , ready_(0)
          {
          }
          ClientReady::ClientReady(uint8 p_ready)
             : type_(NETWORK_MESSAGE_CLIENT_READY)
             , ready_(p_ready)
          {
          }
          bool ClientReady::read(NetworkStreamReader& reader)
          {
              return serialize(reader);
          }
          bool ClientReady::write(NetworkStreamWriter& writer)
          {
              return serialize(writer);
          }

          ServerStatus::ServerStatus()
              : type_(NETWORK_MESSAGE_SERVER_STATUS)
              , ready_(0)
              , player_count_(0)
              , game_start_tick_(0)
              , client_id_(0)
          {
          }
          ServerStatus::ServerStatus(uint8 p_ready, uint8 p_player_count, const uint8 p_id, uint32 p_game_start_timer)
              : type_(NETWORK_MESSAGE_SERVER_STATUS)
              , ready_(p_ready)
              , player_count_(p_player_count)
              , game_start_tick_(p_game_start_timer)
              , client_id_(p_id)
          {
          }
          bool ServerStatus::read(NetworkStreamReader& reader)
          {
              return serialize(reader);
          }
          bool ServerStatus::write(NetworkStreamWriter& writer)
          {
              return serialize(writer);
          }
          ReplicatePlayers::ReplicatePlayers()
              : type_(NETWORK_MESSAGE_PLAYER_REPLICATIONS)
              , player_active_mask_(0)
              , replica_positions_(0)
              , tick_(0)
          {
          }
          ReplicatePlayers::ReplicatePlayers(uint8 p_player_active_mask, uint64 p_player_positions, uint32 p_tick)
              : type_(NETWORK_MESSAGE_PLAYER_REPLICATIONS)
              , player_active_mask_(p_player_active_mask)
              , replica_positions_(p_player_positions)
              , tick_(p_tick)
          {
          }
          bool ReplicatePlayers::read(NetworkStreamReader& reader)
          {
              return serialize(reader);
          }
          bool ReplicatePlayers::write(NetworkStreamWriter& writer)
          {
              return serialize(writer);
          }
          Bomb::Bomb()
              : type_(NETWORK_MESSAGE_BOMB)
              , x_(0)
              , y_(0)
              , target_tick_(0)
              , planter_id_(0)
          {
          }
          Bomb::Bomb(const uint8 p_x, const uint8 p_y, uint32 p_target_tick, const uint8 p_planter_id)
              : type_(NETWORK_MESSAGE_BOMB)
              , x_(p_x)
              , y_(p_y)
              , target_tick_(p_target_tick)
              , planter_id_(p_planter_id)
          {
          }
          bool Bomb::read(NetworkStreamReader& reader)
          {
              return serialize(reader);
          }
          bool Bomb::write(NetworkStreamWriter& writer)
          {
              return serialize(writer);
          }
          GameStatus::GameStatus()
             : type_(NETWORK_MESSAGE_GAME_STATUS)
             , game_end_tick_(0)
             , game_over_(0)
          {
          }
          GameStatus::GameStatus(uint8 p_game_over, uint32 p_game_end_tick)
              : type_(NETWORK_MESSAGE_GAME_STATUS)
              , game_end_tick_(p_game_end_tick)
              , game_over_(p_game_over)
          {
          }
          bool GameStatus::read(NetworkStreamReader& reader)
          {
              return serialize(reader);
          }
          bool GameStatus::write(NetworkStreamWriter& writer)
          {
              return serialize(writer);
          }
}
   } // !network
} // !messages
