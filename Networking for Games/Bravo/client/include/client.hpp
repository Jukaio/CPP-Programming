// client.hpp

#include <bravo.hpp>
#include <network.hpp>
#include <protocol.hpp>

using namespace bravo;

#define DISCONNECT_ATTEMPTS 5

struct NetworkClient
{
	enum class State
	{
		DISCONNECTED,
		REQUESTING,
		RESPONDING,
		CONNECTED,
		DISCONNECTING
	};

	NetworkClient();

	bool initialise();
	void shutdown();

	bool is_connected()		const;
	bool is_connecting()	const;
	bool is_disconnected()	const;
	bool is_disconnecting() const;

	void connect(const IPAddress& p_endpoint);
	void disconnect();

	void update();
	void receive();
	void send();

	void process(const ProtocolChallengePacket& p_packet);
	void process(const ProtocolRejectedPacket& p_packet);
	void process(const ProtocolDataPacket& p_packet);
	void process(const ProtocolDisconnectPacket& p_packet);

	void transmit(const IPAddress& p_address,
				  const NetworkStream& p_stream);

	UDPSocket	m_socket;
	IPAddress	m_endpoint;
	State		m_state;
	uint16		m_sequence;
	uint16		m_ack;
	uint64		m_key;
	uint64		m_challenge;
	int32		m_disconnect_counter;
	Time		m_last_send_time;
	Time		m_last_receive_time;
};

