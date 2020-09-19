// main.cc

#include <cstdio>
#include "server.hpp"

int main(int argc, char **argv)
{
	Network network;
	IPAddress local(IPAddress::ANY_HOST, 54345);
	NetworkServer server;
	if(!server.initialise(local))
	{
		printf("ERR: COULD NOT INITIALISE NETWORK SERVER\n");
		return -1;
	}

	printf("NFO: SERVER RUNNING AT %s\n", local.as_string());
	while(true)
	{
		server.update();
	}

   return 0;
}
