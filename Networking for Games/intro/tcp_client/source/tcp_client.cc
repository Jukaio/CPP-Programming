// tcp_client.cc

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

static void
on_info(const char *message)
{
   printf("NFO: %s\n", message);
}

int main(int argc, char **argv)
{
	WSADATA data;
	if(WSAStartup(MAKEWORD(2, 2), &data))
		return on_error("Could not initialise Winsock!\n");

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET)
		return on_error("Could not create socket!\n");

	// 127.0.0.1 = local IP
	// 54321 = Port

	sockaddr_in remote = {};
	remote.sin_family = AF_INET;
	remote.sin_port = htons(8080);
	remote.sin_addr.S_un.S_un_b.s_b1 = 127;
	remote.sin_addr.S_un.S_un_b.s_b2 = 0;
	remote.sin_addr.S_un.S_un_b.s_b3 = 0;
	remote.sin_addr.S_un.S_un_b.s_b4 = 1;

	if(connect(sock, (const sockaddr*)&remote, sizeof(remote)) == SOCKET_ERROR)
		return on_error("Could not connect to remote");

	char message[128] = "Hello Server";
	size_t s = send(sock, message, (int)strlen(message), 0);
	if(s == SOCKET_ERROR)
		return on_error("Could not send message to Server");

	int r = recv(sock, message, sizeof(message) - 1, 0);
	if(r == SOCKET_ERROR)
		return on_error("Could not receive message from Server");
	else if(r == 0)
		on_info("Connection closed!");

	printf("Response: %s", message);

	shutdown(sock, SD_BOTH);
	closesocket(sock);
	WSACleanup();
	sock = INVALID_SOCKET;

	return 0;
}
