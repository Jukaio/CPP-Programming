// udp_client.cc

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

static int
on_error(const char *message)
{
   int ec = WSAGetLastError();
   printf("ERR: %s (%d)!\n", message, ec);
   return -1;
}

int main(int argc, char **argv)
{
	WSADATA data;
	// Socket sock;
	// sockadd_in server_address;
	// int server_length

	if(WSAStartup(MAKEWORD(2, 2), &data))
		return on_error("Could not initialise Winsock!\n");

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sock == INVALID_SOCKET)
		return on_error("Could not create socket!\n");

	// 127.0.0.1 = local IP
	// 54321 = Port

	struct sockaddr_in remote = {};
	remote.sin_family = AF_INET;
	remote.sin_port = htons(8080);
	remote.sin_addr.S_un.S_un_b.s_b1 = 127;
	remote.sin_addr.S_un.S_un_b.s_b2 = 0;
	remote.sin_addr.S_un.S_un_b.s_b3 = 0;
	remote.sin_addr.S_un.S_un_b.s_b4 = 1;


	int server_length = sizeof(remote);
	char message[128] = "Hello Server";

	printf("Client sends: %s\n", message);
	size_t s = sendto(sock, 
					  message, 
					  (int)strlen(message), 
					  0,
					  (const sockaddr*)&remote,
					  server_length);
	if(s == SOCKET_ERROR)
		return on_error("Could not send message to Server");

	int r = recvfrom(sock, 
					 message, 
					 sizeof(message) -1, 
					 0, 
					 (struct sockaddr*)&remote, 
					 &server_length);
	if(r == SOCKET_ERROR)
		return on_error("Could not receive message from Server");

	printf("Client receives: %s\n", message);

	shutdown(sock, SD_BOTH);
	closesocket(sock);
	WSACleanup();
	sock = INVALID_SOCKET;

	return 0;
}
