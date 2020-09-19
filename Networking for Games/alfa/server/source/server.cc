// server.cc

#include <cstdio>
#include <alfa.hpp>

using namespace alfa;

int main(int argc, char** argv)
{
    const Network network;

    const IPAddress local(9, 0, 0, 1, 8080);
    UDPSocket socket;
    if(!socket.open(local))
    {
        return 0;
    }

    IPAddress remote;
    Time last_receive_time = Time::now();

    bool running = true;
    while(running)
    {
        uint8 buffer[128] = {};
        int32 length = sizeof(buffer);

        //Receive data from client
        if(socket.receive(remote, buffer, length))
        {
            last_receive_time = Time::now();
            ByteStreamReader reader(length, buffer);

            MessageHelloWorld receivedMessage;
            receivedMessage.read(reader);

            //Send response to client
            ByteStreamWriter writer(length, buffer);
            MessageHelloWorld messageToSend;
            messageToSend.m_ack = receivedMessage.m_sequence;
            messageToSend.write(writer);

            socket.send(remote, writer.data(), writer.length());

            //Print stuff
            printf("Received: %s - #%u - %f\n",
                   remote.as_string(),
                   receivedMessage.m_sequence,
                   last_receive_time.as_seconds());
        }
    }

    return 0;
}