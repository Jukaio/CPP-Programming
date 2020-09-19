// main.cc

#include <cstdio>
#include "client.hpp"

int main(int argc, char **argv)
{
	Network network;
	NetworkClient client;
	if(!client.initialise())
	{
		printf("ERR: COULD NOT INITIALISE NETWORK CLIENT\n");
		return -1;
	}

	IPAddress remote(9, 0, 0, 1, 54345);
	printf("Connecting to server at %s\n", remote.as_string());
	client.connect(remote);
	while(client.is_connecting() || client.is_connected())
	{
		client.update();
		Time::sleep(Time(0.1));
	}

   return 0;
}
