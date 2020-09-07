// udp_server.cc

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define DEFAULT_BUFFER 512

static int
on_error(const char *message)
{
   int ec = WSAGetLastError();
   printf("ERR: %s (%d)!\n", message, ec);
   return -1;
}

void invert_string_in_buffer(char* buffer, int length)
{
	char temp[DEFAULT_BUFFER];
	if(strcpy_s(temp, sizeof(temp), buffer))
		return;

	for(int i = 0; i < length; i++)
	{
		buffer[i] = temp[(length - 1) - i];
	}
}

int main(int argc, char **argv)
{
	WSADATA				data;
	struct sockaddr_in	client_address;
	struct addrinfo		server_address;
	int					error_result					= NULL;
	int					send_result						= NULL;
	int					receive_result					= NULL;
	int					client_length					= sizeof(client_address);
	struct addrinfo*	server_address_results			= NULL;
	SOCKET				sock							= INVALID_SOCKET;
	char				receive_buffer[DEFAULT_BUFFER]	= "\0";

	if(WSAStartup(MAKEWORD(2, 2), &data))
		return on_error("Could not initialise Winsock!\n");

	memset(&client_address, 0, sizeof(client_address));
	memset(&server_address, 0, sizeof(server_address));
	server_address.ai_family = AF_INET;
	server_address.ai_socktype = SOCK_DGRAM;
	server_address.ai_protocol = IPPROTO_UDP;
	server_address.ai_flags = AI_PASSIVE;

	error_result = getaddrinfo(NULL,
							   "8080",
							   &server_address,
							   &server_address_results);
	if(error_result != 0)
		return on_error("Could not get address info!\n");

	sock = socket(server_address_results->ai_family,
				  server_address_results->ai_socktype,
				  server_address_results->ai_protocol);
	if(sock == INVALID_SOCKET)
		return on_error("Invalid Socket");

	error_result = bind(sock,
						server_address_results->ai_addr,
						(int)server_address_results->ai_addrlen);
	if(error_result == SOCKET_ERROR)
	{
		WSACleanup();
		return on_error("Could not bind address socket!\n");
	}

	receive_result = recvfrom(sock, 
							  receive_buffer, 
							  sizeof(receive_buffer), 
							  0,
							  (struct sockaddr*) &client_address,
							  &client_length);
	if(receive_result == SOCKET_ERROR) // Send back
	{
		WSACleanup();
		return on_error("Could not receive from client!\n");
	}

	printf("Server receives: %s\n", receive_buffer);
	invert_string_in_buffer(receive_buffer, receive_result);

	printf("Server sends:  %s\n", receive_buffer);
	send_result = sendto(sock, 
						 receive_buffer, 
						 receive_result, 
						 0,
						 (struct sockaddr*)&client_address,
						 client_length);
	if(send_result == SOCKET_ERROR)
	{
		WSACleanup();
		return on_error("Could not send to client!\n");
	}
	
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	WSACleanup();
	return 0;
}
