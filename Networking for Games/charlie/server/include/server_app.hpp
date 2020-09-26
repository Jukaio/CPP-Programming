// server_app.hpp

#ifndef SERVER_APP_HPP_INCLUDED
#define SERVER_APP_HPP_INCLUDED

#include <charlie_application.hpp>

using namespace charlie;

struct ClientList
{
	struct Client
	{
		int32 m_id{ -1 };
		uint64 m_connection{0};
		Buffer<TimeState> m_time_state;
	};

	ClientList()
		: m_next(0)
	{

	}

	int64 add_client(const uint64 connection)
	{
		const int32 id = m_next++;
		m_clients.push_back({ id, connection });
		return id;
	}

	void remove_client(const uint64 connection)
	{
		auto it = m_clients.begin();
		while (it != m_clients.end()) {
			if ((*it).m_connection == connection) {
				m_clients.erase(it);
				break;
			}
			++it;
		}
	}

	Client* find_client(const uint64 p_connection)
	{
		for (auto& client : m_clients)
		{
			if (client.m_connection == p_connection)
				return &client;
		}
		return nullptr;
	}

	int32 m_next;
	DynamicArray<Client> m_clients;
};

struct ServerApp final : Application, network::IServiceListener, network::IConnectionListener {
   ServerApp();

   // note: Application
   virtual bool on_init();
   virtual void on_exit();
   virtual bool on_tick(const Time &dt);
   virtual void on_draw();

   // note: IServiceListener
   virtual void on_timeout(network::Connection *connection);
   virtual void on_connect(network::Connection *connection);
   virtual void on_disconnect(network::Connection *connection);

   // note: IConnectionListener 
   virtual void on_acknowledge(network::Connection *connection, const uint16 sequence);
   virtual void on_receive(network::Connection *connection, network::NetworkStreamReader &reader);
   virtual void on_send(network::Connection *connection, const uint16 sequence, network::NetworkStreamWriter &writer);

   Time accumulator_;
   uint32 ticks_;

   ClientList m_client_list;
   gameplay::Entity entity_;
};

#endif // !SERVER_APP_HPP_INCLUDED
