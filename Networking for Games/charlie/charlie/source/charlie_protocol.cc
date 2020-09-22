// charlie_protocol.cc

#include "charlie_protocol.hpp"
#include "charlie_network.hpp"

namespace charlie {
   namespace network {
      ProtocolRequestPacket::ProtocolRequestPacket()
         : type_(PROTOCOL_PACKET_REQUEST)
         , protocol_(PROTOCOL_ID)
         , version_(PROTOCOL_VERSION)
         , key_(0)
      {
      }

      ProtocolRequestPacket::ProtocolRequestPacket(const uint64 key)
         : type_(PROTOCOL_PACKET_REQUEST)
         , protocol_(PROTOCOL_ID)
         , version_(PROTOCOL_VERSION)
         , key_(key)
      {
      }

      bool ProtocolRequestPacket::read(NetworkStreamReader &reader)
      {
         return serialize(reader);
      }

      bool ProtocolRequestPacket::write(NetworkStreamWriter &writer)
      {
         return serialize(writer);
      }

      ProtocolChallengePacket::ProtocolChallengePacket()
         : type_(PROTOCOL_PACKET_CHALLENGE)
         , challenge_(0)
      {
      }

      ProtocolChallengePacket::ProtocolChallengePacket(const uint64 challenge)
         : type_(PROTOCOL_PACKET_CHALLENGE)
         , challenge_(challenge)
      {
      }

      bool ProtocolChallengePacket::read(NetworkStreamReader &reader)
      {
         return serialize(reader);
      }

      bool ProtocolChallengePacket::write(NetworkStreamWriter &writer)
      {
         return serialize(writer);
      }

      ProtocolResponsePacket::ProtocolResponsePacket()
         : type_(PROTOCOL_PACKET_RESPONSE)
         , response_(0)
      {
      }

      ProtocolResponsePacket::ProtocolResponsePacket(const uint64 response)
         : type_(PROTOCOL_PACKET_RESPONSE)
         , response_(response)
      {
      }

      bool ProtocolResponsePacket::read(NetworkStreamReader &reader)
      {
         return serialize(reader);
      }

      bool ProtocolResponsePacket::write(NetworkStreamWriter &writer)
      {
         return serialize(writer);
      }

      ProtocolRejectedPacket::ProtocolRejectedPacket()
         : type_(PROTOCOL_PACKET_REJECTED)
         , reason_(0)
      {
      }

      ProtocolRejectedPacket::ProtocolRejectedPacket(const uint8 reason)
         : type_(PROTOCOL_PACKET_REJECTED)
         , reason_(reason)
      {
      }

      bool ProtocolRejectedPacket::read(NetworkStreamReader &reader)
      {
         return serialize(reader);
      }

      bool ProtocolRejectedPacket::write(NetworkStreamWriter &writer)
      {
         return serialize(writer);
      }

      ProtocolDataPacket::ProtocolDataPacket()
         : type_(PROTOCOL_PACKET_DATA)
         , sequence_(0)
         , acknowledge_(0)
         , ack_bits_(0)
         , ticks_(0)
      {
      }

      ProtocolDataPacket::ProtocolDataPacket(const uint16 sequence,
                                             const uint16 acknowledge,
                                             const uint32 ack_bits,
                                             const uint32 ticks)
         : type_(PROTOCOL_PACKET_DATA)
         , sequence_(sequence)
         , acknowledge_(acknowledge)
         , ack_bits_(ack_bits)
         , ticks_(ticks)
      {
      }

      bool ProtocolDataPacket::read(NetworkStreamReader &reader)
      {
         return serialize(reader);
      }

      bool ProtocolDataPacket::write(NetworkStreamWriter &writer)
      {
         return serialize(writer);
      }

      ProtocolDisconnectPacket::ProtocolDisconnectPacket()
         : type_(PROTOCOL_PACKET_DISCONNECT)
      {
      }

      bool ProtocolDisconnectPacket::read(NetworkStreamReader &reader)
      {
         return serialize(reader);
      }

      bool ProtocolDisconnectPacket::write(NetworkStreamWriter &writer)
      {
         return serialize(writer);
      }
   } // !network
} // !charlie
