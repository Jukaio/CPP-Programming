// charlie_protocol.hpp

#ifndef CHARLIE_PROTOCOL_HPP_INCLUDED
#define CHARLIE_PROTOCOL_HPP_INCLUDED

#include <charlie.hpp>

namespace charlie {
   namespace network {
      struct NetworkStreamReader;
      struct NetworkStreamWriter;

      static constexpr uint32 PROTOCOL_ID = 'CHRL';
      static constexpr uint32 PROTOCOL_VERSION = 'v.01';

      static constexpr double CONNECTION_SEND_RATE = 1.0 / 5.0;
      static constexpr double CONNECTION_TIMEOUT_LIMIT = 5.0;

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
         REJECT_REASON_UNKNOWN,
         REJECT_REASON_PROTOCOL,
         REJECT_REASON_VERSION,
         REJECT_REASON_CHALLENGE,
         REJECT_REASON_SERVER_FULL,
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
         explicit ProtocolDataPacket(const uint16 sequence,
                                     const uint16 ack,
                                     const uint32 ack_bits,
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
            result &= stream.serialize(ack_bits_);
            result &= stream.serialize(ticks_);
            return result;
         }

         uint8  type_;
         uint16 sequence_;
         uint16 acknowledge_;
         uint32 ack_bits_;
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
   } // !network
} // !charlie

#endif // !CHARLIE_PROTOCOL_HPP_INCLUDED
