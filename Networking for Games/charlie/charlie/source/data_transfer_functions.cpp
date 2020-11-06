// data_transfer_functions.cpp

#include "charlie_messages.hpp"
#include "charlie_network.hpp"

charlie::network::DataTransferFunctions::DataTransferFunctions()
{
	using namespace network::message;

	//m_send_flag = 0;
	m_send_functions.resize(NETWORK_MESSAGE_COUNT);
	m_receive_functions.resize(NETWORK_MESSAGE_COUNT);
}

void charlie::network::DataTransferFunctions::send_messages(network::Connection* connection, const uint16 sequence, network::NetworkStreamWriter& writer)
{
	using namespace network::message;
	
	uint8* content_at = writer.at_;

	ContentInfo content_info;
	if(!content_info.write(writer))
		assert(!"failed to write message!");

	uint16 flag = 0;
	uint32 index = 0;
	while(index < NETWORK_MESSAGE_COUNT)
	{
		if(m_send_functions[index])
		{
			m_send_functions[index](connection, sequence, writer);
			flag |= (1 << index);
		}
		index++;
	}
	ContentInfo::set_flag(content_at, flag);
}
void charlie::network::DataTransferFunctions::receive_messages(network::Connection* connection, network::NetworkStreamReader& reader)
{
	using namespace network::message;

	if(reader.peek() == NETWORK_MESSAGE_CONTENT_INFO)
	{
		int limit_count = 254;
		ContentInfo content_info;
		if(!content_info.read(reader))
			assert(!"failed to read message!");

		uint32 index = 0;
		while(index < NETWORK_MESSAGE_COUNT)
		{
			if(content_info.m_content_flag & (1 << index))
			{
				while(reader.peek() == index)
				{ 
					if(m_receive_functions[index])
						m_receive_functions[index](connection, reader);
					limit_count--;
					if(limit_count < 0)
					{
						// message is useless to the state - return 
						// TODO: instead of bruteforcing this, maybe hint something in content?
						return;
					}
				}
			}
			index++;
		}
	}
}

void charlie::network::DataTransferFunctions::set_send_function(uint16 p_index, SendFunction p_function)
{
	if(p_index < network::message::NETWORK_MESSAGE_COUNT)
	{
		m_send_functions[p_index] = p_function;
	}
}

void charlie::network::DataTransferFunctions::set_receive_function(uint16 p_index, ReceiveFunction p_function)
{
	if(p_index < network::message::NETWORK_MESSAGE_COUNT)
	{
		m_receive_functions[p_index] = p_function;
	}
}
