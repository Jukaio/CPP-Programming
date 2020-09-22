// client.cc

#include "client.hpp"
#include <cstdio>

#define array_size(x) (sizeof(x)/sizeof(x[0]))

namespace {
   // source: github.com/ademakov/RNG/blob/master/rng.h
   static uint64 _rseed;
   static struct Random {
      Random(const uint64 seed = (const uint64)&_rseed)
         : state_(seed)
      {
      }

      uint64 operator()() {
         state_ += 0x9e3779b97f4a7c15ull;
         uint64 z = state_;
         z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
         z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
         return z ^ (z >> 31);
      }

      uint64 state_;
   } random;
} // !anon

NetworkClient::NetworkClient()
    : m_state(State::DISCONNECTED)
    , m_key(0)
    , m_disconnect_counter(0)
    , m_sequence(0)
    , m_ack(0)
    , m_challenge(0)
{
}

bool NetworkClient::initialise()
{
    if(!m_socket.open())
    {
        return false;
    }
    return true;
}

void NetworkClient::shutdown()
{
    m_socket.close();
}

bool NetworkClient::is_connected() const
{
    return m_state == State::CONNECTED;
}

bool NetworkClient::is_connecting() const
{
    return m_state == State::REQUESTING ||
           m_state == State::RESPONDING;
}

bool NetworkClient::is_disconnected() const
{
    return m_state == State::DISCONNECTED;
}

bool NetworkClient::is_disconnecting() const
{
    return m_state == State::DISCONNECTING;
}

void NetworkClient::connect(const IPAddress& p_endpoint)
{
    if(!is_disconnected())
        return;

    m_endpoint = p_endpoint;
    m_state = State::REQUESTING;
    m_key = random();
}

void NetworkClient::disconnect()
{
    if(is_disconnected()) 
        return;

    m_state = State::DISCONNECTING;
    m_disconnect_counter = 0;
}

void NetworkClient::update()
{
    receive();
    send();
}

void NetworkClient::receive()
{
    if(is_disconnected())
        return;

    IPAddress       address;
    NetworkStream   stream;

    if(m_socket.receive(address, stream.buffer_, stream.length_))
    {
        if(address != m_endpoint)
            return;

        m_last_receive_time = Time::now();
        NetworkStreamReader reader(stream);

        switch(reader.peek())
        {
            case PROTOCOL_PACKET_CHALLENGE:
            {
               ProtocolChallengePacket packet;
               if(!packet.read(reader))
                   assert(!"FAILED TO READ CHALLENGE PACKET\n");
               process(packet);
            }  break;
            case PROTOCOL_PACKET_REJECTED:
            {
                ProtocolRejectedPacket packet;
                if(!packet.read(reader))
                    assert(!"FAILED TO READ REJECT PACKET\n");
                process(packet);
            }   break;
                
            case PROTOCOL_PACKET_DATA:
            {    
                ProtocolDataPacket packet;
                if(!packet.read(reader))
                    assert(!"FAILED TO READ DATA PACKET\n");
                process(packet);
            }   break;
            case PROTOCOL_PACKET_DISCONNECT:
            {
                ProtocolDisconnectPacket packet;
                if(!packet.read(reader))
                    assert(!"FAILED TO READ DISCONNECT PACKET\n");
                process(packet);
            }   break;
        }

    }

}

void NetworkClient::send()
{
    if(is_disconnected())
        return;
    const Time current_time = Time::now();
    if((current_time - m_last_send_time) >= Time(0.2))
    {
        NetworkStream       stream;
        NetworkStreamWriter writer(stream);

        switch(m_state)
        {
            case NetworkClient::State::REQUESTING:
            {
                ProtocolRequestPacket packet(m_key);
                packet.write(writer);
                transmit(m_endpoint, stream);
            }   break;
            case NetworkClient::State::RESPONDING:
            {
                ProtocolResponsePacket packet(m_key ^ m_challenge);
                packet.write(writer);
                transmit(m_endpoint, stream);
            }   break;
            case NetworkClient::State::CONNECTED:
            {
                const Time delta_time = current_time - m_last_receive_time;
                const uint32 process_time = (uint32)delta_time.as_ticks();
                ProtocolDataPacket packet(m_sequence++, 
                                          m_ack, 
                                          process_time);
                packet.write(writer);
                transmit(m_endpoint, stream);
            }   break;
            case NetworkClient::State::DISCONNECTING:
            {
                if(m_disconnect_counter == DISCONNECT_ATTEMPTS)
                {
                    m_state = State::DISCONNECTED;
                    printf("Disconnected\n");
                    return;
                }

                ProtocolDisconnectPacket packet;
                packet.write(writer);
                transmit(m_endpoint, stream);
                m_disconnect_counter++;
            }   break;
            default:
                break;
        }


        m_last_send_time = current_time;
    }
}

void NetworkClient::process(const ProtocolChallengePacket& p_packet)
{
    if(m_state == State::REQUESTING)
    {
        m_state = State::RESPONDING;
        m_challenge = p_packet.challenge_;
    }
}

void NetworkClient::process(const ProtocolRejectedPacket& p_packet)
{
    if(m_state == State::REQUESTING ||
       m_state == State::RESPONDING)
    {
        m_state = State::DISCONNECTED;
        printf("Rejected %d\n", p_packet.reason_);
        // TODO: Reset values;
    }
}

void NetworkClient::process(const ProtocolDataPacket& p_packet)
{
    if(m_state == State::RESPONDING)
    {
        m_state = State::CONNECTED;
        printf("Connected!\n");
    }

    if(m_state == State::CONNECTED)
    {
        m_ack = p_packet.sequence_;
        printf("SEQ: %d - ACK: %d - TICKS: %d\n", m_sequence, m_ack, p_packet.ticks_);
    }
}

void NetworkClient::process(const ProtocolDisconnectPacket& p_packet)
{
    if(m_state == State::CONNECTED)
    {
        m_state = State::DISCONNECTED;

        // TODO: Reset values
        printf("Disonnected!\n");
    }
}

void NetworkClient::transmit(const IPAddress& p_address, 
                             const NetworkStream& p_stream)
{
    if(!m_socket.send(p_address, p_stream.buffer_, p_stream.length_))
    {
        int error_code = Network::get_error();
        if(Network::is_critical(error_code))
            assert(!"Crtical Error");
    }
}
