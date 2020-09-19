// protocol.hpp

#ifndef PROTOCOL_HPP_INCLUDED
#define PROTOCOL_HPP_INCLUDED

#include "bravo.hpp"
#include "network.hpp"

namespace bravo {
   static constexpr uint32 NETWORK_PROTOCOL_ID      = 'BRVO';
   static constexpr uint32 NETWORK_PROTOCOL_VERSION = 'v.01';

   enum ProtocolPacketType {
      PROTOCOL_PACKET_REQUEST,
      PROTOCOL_PACKET_CHALLENGE,
      PROTOCOL_PACKET_RESPONSE,
      PROTOCOL_PACKET_REJECTED,
      PROTOCOL_PACKET_DATA,
      PROTOCOL_PACKET_DISCONNECT,
      PROTOCOL_PACKET_COUNT,
   };

   enum RejectedReason {
      REJECTION_REASON_UNKNOWN,
      REJECTION_REASON_PROTOCOL,
      REJECTION_REASON_VERSION,
      REJECTION_REASON_CHALLENGE,
      REJECTION_REASON_FULL,
   };

   struct ProtocolRequestPacket {
      ProtocolRequestPacket();
      explicit ProtocolRequestPacket(const uint64 key);

      bool read(NetworkStreamReader &reader);
      bool write(NetworkStreamWriter &writer);

      template <typename Stream>
      bool serialize(Stream &stream)
      {
         bool result = true;
         result &= stream.serialize(type_);
         result &= stream.serialize(protocol_);
         result &= stream.serialize(version_);
         result &= stream.serialize(key_);
         return result;
      }

      uint8 type_;
      uint32 protocol_;
      uint32 version_;
      uint64 key_;
   };

   struct ProtocolChallengePacket {
      ProtocolChallengePacket();
      explicit ProtocolChallengePacket(const uint64 challenge);

      bool read(NetworkStreamReader &reader);
      bool write(NetworkStreamWriter &writer);

      template <typename Stream>
      bool serialize(Stream &stream)
      {
         bool result = true;
         result &= stream.serialize(type_);
         result &= stream.serialize(challenge_);
         return result;
      }

      uint8 type_;
      uint64 challenge_;
   };

   struct ProtocolResponsePacket {
      ProtocolResponsePacket();
      explicit ProtocolResponsePacket(const uint64 response);

      bool read(NetworkStreamReader &reader);
      bool write(NetworkStreamWriter &writer);

      template <typename Stream>
      bool serialize(Stream &stream)
      {
         bool result = true;
         result &= stream.serialize(type_);
         result &= stream.serialize(response_);
         return result;
      }

      uint8 type_;
      uint64 response_;
   };

   struct ProtocolRejectedPacket {
      ProtocolRejectedPacket();
      explicit ProtocolRejectedPacket(const uint8 reason);

      bool read(NetworkStreamReader &reader);
      bool write(NetworkStreamWriter &writer);

      template <typename Stream>
      bool serialize(Stream &stream)
      {
         bool result = true;
         result &= stream.serialize(type_);
         result &= stream.serialize(reason_);
         return result;
      }

      uint8 type_;
      uint8 reason_;
   };

   struct ProtocolDataPacket {
      ProtocolDataPacket();
      ProtocolDataPacket(const uint16 sequence,
                 const uint16 ack,
                 const uint32 ticks);

      bool read(NetworkStreamReader &reader);
      bool write(NetworkStreamWriter &writer);

      template <typename Stream>
      bool serialize(Stream &stream)
      {
         bool result = true;
         result &= stream.serialize(type_);
         result &= stream.serialize(sequence_);
         result &= stream.serialize(acknowledge_);
         result &= stream.serialize(ticks_);
         return result;
      }

      uint8 type_;
      uint16 sequence_;
      uint16 acknowledge_;
      uint32 ticks_;
   };

   struct ProtocolDisconnectPacket {
      ProtocolDisconnectPacket();

      bool read(NetworkStreamReader &reader);
      bool write(NetworkStreamWriter &writer);

      template <typename Stream>
      bool serialize(Stream &stream)
      {
         bool result = true;
         result &= stream.serialize(type_);
         return result;
      }

      uint8 type_;
   };
} // !bravo

#endif // !PACKETS_HPP_INCLUDED
