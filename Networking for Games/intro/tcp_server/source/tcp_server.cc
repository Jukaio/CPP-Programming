// tcp_server.cc

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_BUFFER 512

static int
on_error(const char *message)
{
   int ec = WSAGetLastError();
   printf("ERR: %s (%d)!\n", message, ec);
   return -1;
}

static void
on_info(const char *message)
{
   printf("NFO: %s\n", message);
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
	WSADATA data;

	SOCKET listen_socket = INVALID_SOCKET;
	SOCKET client_socket = INVALID_SOCKET;

	struct addrinfo* address_results = NULL;
	struct addrinfo address_hints;

	int error_result; 
	int send_result;
	int receive_result;

	char receive_buffer[DEFAULT_BUFFER] = "";

	if(WSAStartup(MAKEWORD(2, 2), &data))
		return on_error("Could not initialise Winsock!\n");

	memset(&address_hints, 0, sizeof(address_hints));
	address_hints.ai_family = AF_INET;
	address_hints.ai_socktype = SOCK_STREAM;
	address_hints.ai_protocol = IPPROTO_TCP;
	address_hints.ai_flags = AI_PASSIVE;

	error_result = getaddrinfo(NULL, 
							  "8080", 
							  &address_hints, 
							  &address_results);
	if(error_result != 0)
		return on_error("Could not get address info!\n");


	listen_socket = socket(address_results->ai_family, 
						   address_results->ai_socktype, 
						   address_results->ai_protocol);
	if(listen_socket == INVALID_SOCKET)
	{ 
		freeaddrinfo(address_results);
		WSACleanup();
		return on_error("Could not create listener socket!\n");
	}

	error_result = bind(listen_socket, 
						address_results->ai_addr,
						(int) address_results->ai_addrlen);
	if(error_result == SOCKET_ERROR)
	{
		freeaddrinfo(address_results);
		closesocket(listen_socket);
		WSACleanup();
		return on_error("Could not bind listener socket!\n");
	}
	freeaddrinfo(address_results);

	error_result = listen(listen_socket, SOMAXCONN);
	if(error_result == SOCKET_ERROR)
	{
		closesocket(listen_socket);
		WSACleanup();
		return on_error("Could not listen!\n");
	}

	client_socket = accept(listen_socket, NULL, NULL);
	if(client_socket == INVALID_SOCKET)
	{
		closesocket(listen_socket);
		WSACleanup();
		return on_error("Could not create client socket!\n");
	}

	receive_result = recv(client_socket, receive_buffer, sizeof(receive_buffer), 0);
	if(receive_result > 0) // Send back
	{
		printf("%s", receive_buffer);
		invert_string_in_buffer(receive_buffer, receive_result);
		send_result = send(client_socket, receive_buffer, receive_result, 0);
		if(send_result == SOCKET_ERROR)
		{
			closesocket(listen_socket);
			WSACleanup();
			return on_error("Could not send to client!\n");
		}
	}
	else if(error_result == 0) // Terminate connection
	{
		on_info("Connection closed!");
	}
	else
	{
		closesocket(client_socket);
		WSACleanup();
		return on_error("Could not receive from client!\n");
	}

	error_result = shutdown(client_socket, SD_SEND);
	if(error_result == SOCKET_ERROR)
	{
		closesocket(listen_socket);
		WSACleanup();
		return on_error("Shutdown failed!\n");
	}

	closesocket(client_socket);
	closesocket(listen_socket);
	WSACleanup();

   return 0;
}
