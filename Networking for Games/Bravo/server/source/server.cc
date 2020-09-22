// server.cc

#include "server.hpp"
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

NetworkServer::Connection::Connection()
    : m_state(State::DISCONNECTED)
    , m_key(0)
    , m_challenge(0)
    , m_ack(0)
{
}

bool NetworkServer::Connection::is_connected() const
{
    return m_state == State::CONNECTED;
}

bool NetworkServer::Connection::is_connecting() const
{
    return m_state == State::CHALLENGE;
}

bool NetworkServer::Connection::is_disconnected() const
{
    return m_state == State::DISCONNECTED;
}

bool NetworkServer::Connection::is_disconnecting() const
{
    return m_state == State::DISCONNECTING;
}

NetworkServer::NetworkServer()
    : m_sequence(0)
{
}

bool NetworkServer::initialise(const IPAddress& p_address)
{
    if(!m_socket.open(p_address))
        return false;

    return true;
}

void NetworkServer::shutdown()
{
    if(m_socket.is_valid()) 
        m_socket.close();
}

void NetworkServer::update()
{
    receive();
    send();

}

void NetworkServer::send()
{
    if(!m_socket.is_valid())
        return;

    // note: handle pending connections
    const Time current_time = Time::now();
    for(auto& connection : m_pending_connections)
    {
        if(connection.is_connecting())
        { 
            if((current_time - connection.m_last_send_time) >= Time(0.2))
            {
                ProtocolChallengePacket packet(connection.m_challenge);
                send_packet(connection.m_address, packet);
                connection.m_last_send_time = current_time;
            }
        }
    }

    for(auto& connection : m_established_connections)
    {
        if(connection.is_connected())
        {
            if((current_time - connection.m_last_send_time) >= Time(0.2))
            {
                const Time delta_time = current_time - connection.m_last_send_time;
                const uint32 process_time = (uint32)delta_time.as_ticks();
                ProtocolDataPacket packet(m_sequence++, connection.m_ack, process_time);
                send_packet(connection.m_address, packet);
                connection.m_last_send_time = current_time;
            }
        }
    }
}

void NetworkServer::receive()
{
    if(!m_socket.is_valid())
        return;

    IPAddress address;
    NetworkStream stream;
    while(m_socket.receive(address, stream.buffer_, stream.length_))
    {
        NetworkStreamReader reader(stream);

        switch(reader.peek())
        {
            case PROTOCOL_PACKET_REQUEST:
            {
                ProtocolRequestPacket packet;
                if(!packet.read(reader))
                    assert(!"FAILED TO READ PACKET");
                process(address, packet);
            }   break;

            case PROTOCOL_PACKET_RESPONSE:
            {
                ProtocolResponsePacket packet;
                if(!packet.read(reader))
                    assert(!"FAILED TO READ PACKET");
                process(address, packet);
            }   break;
            case PROTOCOL_PACKET_DATA:
            {
                ProtocolDataPacket packet;
                if(!packet.read(reader))
                    assert(!"FAILED TO READ PACKET");
                process(address, packet);
            }   break;
            case PROTOCOL_PACKET_DISCONNECT:
            {
            }   break;
        }

        
        stream.length_ = sizeof(stream.buffer_);
    }
}

void NetworkServer::process(const IPAddress& p_address, 
                            const ProtocolRequestPacket& p_packet)
{
    if(p_packet.protocol_ != NETWORK_PROTOCOL_ID)
    {
        ProtocolRejectedPacket reject(REJECTION_REASON_PROTOCOL);
        send_packet(p_address, reject);
    }
    else if(p_packet.version_ != NETWORK_PROTOCOL_VERSION)
    {
        ProtocolRejectedPacket reject(REJECTION_REASON_VERSION);
        send_packet(p_address, reject);
    }
    for(auto& pending : m_pending_connections)
    {
        if(pending.m_address == p_address)
            return;
    }

    m_pending_connections.push_back({});
    Connection &connection = m_pending_connections.back();
    connection.m_address = p_address;
    connection.m_key = p_packet.key_;
    connection.m_challenge = random();
    connection.m_state = Connection::State::CHALLENGE;
    connection.m_last_receive_time = Time::now();

    printf("NFO: new pending connections fom %s\n", p_address.as_string());
}

void NetworkServer::process(const IPAddress& p_address,
                            const ProtocolResponsePacket& p_packet)
{
    auto it = m_pending_connections.begin();
    while(it != m_pending_connections.end())
    {
        if(p_address == (*it).m_address)
            if((*it).is_disconnected())
            {
                it = m_pending_connections.erase(it);
                return;
            }
            else if((*it).is_connecting())
            {
                if(p_packet.response_ == ((*it).m_key ^ (*it).m_challenge))
                {
                    m_established_connections.push_back(*it);
                    m_established_connections.back().m_state = Connection::State::CONNECTED;
                }
                else
                {
                    ProtocolRejectedPacket reject(REJECTION_REASON_PROTOCOL);
                    send_packet(p_address, reject);
                }
                it = m_pending_connections.erase(it);
                return;
            }
        else
            it++;
    }
}

void NetworkServer::process(const IPAddress& p_address,
                            const ProtocolDataPacket& p_packet)
{
    auto it = m_established_connections.begin();
    while(it != m_established_connections.end())
    {
        if(p_address == (*it).m_address)
            if((*it).is_connected())
            {
                (*it).m_ack = p_packet.sequence_;
                printf("SEQ: %d - ACK: %d - TICKS: %d\n", m_sequence, (*it).m_ack, p_packet.ticks_);
                return;
            }
            else
                it++;
    }
}

void NetworkServer::process(const IPAddress& p_address,
                            const ProtocolDisconnectPacket& p_packet)
{
}

void NetworkServer::send_stream(const IPAddress& p_address, 
                                const NetworkStream& p_stream)
{
    if(!m_socket.send(p_address, p_stream.buffer_, p_stream.length_))
    {
        const int32 error_code = Network::get_error();
        if(Network::is_critical(error_code))
            assert(!"CRITICAL NETWORK ERROR!");
    }
}
