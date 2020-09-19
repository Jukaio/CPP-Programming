// server.hpp

#include <bravo.hpp>
#include <network.hpp>
#include <protocol.hpp>

using namespace bravo;

struct NetworkServer
{
	struct Connection
	{
		enum class State
		{
			DISCONNECTED,
			CHALLENGE,
			CONNECTED,
			DISCONNECTING
		};

		bool is_connected() const;
		bool is_connecting() const;
		bool is_disconnected() const;
		bool is_disconnecting() const;

		Connection();

		IPAddress	m_address;
		State		m_state;
		uint64		m_key;
		uint64		m_challenge;
		Time		m_last_send_time;
		Time		m_last_receive_time;
	};

	NetworkServer();

	bool initialise(const IPAddress &p_address);
	void shutdown();
	void update();
	
	void send();
	void receive();

	void process(const IPAddress& p_address, const ProtocolRequestPacket& p_packet);
	void process(const IPAddress& p_address, const ProtocolResponsePacket& p_packet);
	void process(const IPAddress& p_address, const ProtocolDataPacket& p_packet);
	void process(const IPAddress& p_address, const ProtocolDisconnectPacket& p_packet);

	template <typename T>
	void send_packet(const IPAddress p_address, T& packet)
	{
		NetworkStream stream;
		NetworkStreamWriter writer(stream);
		if(!packet.write(writer))
			assert(!"COULD NOT WRITE PACKET T!");
		send_stream(p_address, stream);
	}

	void send_stream(const IPAddress &address, const NetworkStream &stream);

	UDPSocket m_socket;
	DynamicArray<Connection> m_pending_connections;
	DynamicArray<Connection> m_established_connections;

};
