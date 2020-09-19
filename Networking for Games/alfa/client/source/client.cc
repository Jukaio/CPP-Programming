// client.cc

#include <cstdio>
#include <alfa.hpp>

using namespace alfa;

int main(int argc, char** argv)
{
    const Network network;

    const IPAddress remote(9, 0, 0, 2, 8080);
    IPAddress responseRemote;
    UDPSocket socket;
    if(!socket.open())
    {
        return 0;
    }

    Time last_send_time = Time::now();
    Time last_receive_time = Time::now();
    uint32 sequence = 0;

    bool running = true;
    while(running)
    {
        uint8 buffer[128] = {};
        int32 length = sizeof(buffer);

        const Time current_time = Time::now();
        // Send data to server
        if((current_time - last_send_time) >= Time(1.0))
        {
            ByteStreamWriter writer(length, buffer);

            MessageHelloWorld message;
            message.m_sequence = sequence++;
            message.write(writer);

            if(!socket.send(remote, writer.data(), writer.length()))
                return 0;

            last_send_time = Time::now();
        }

        //Receive Response from server
        if(socket.receive(responseRemote, buffer, length))
        {
            last_receive_time = Time::now();
            ByteStreamReader reader(length, buffer);

            MessageHelloWorld receivedMessage;
            receivedMessage.read(reader);

            //Calculate RTT
            float ping = last_receive_time.as_milliseconds() - last_send_time.as_milliseconds();

            //Print stuff
            printf("Received ack#%u - %f\n",
                   receivedMessage.m_ack,
                   ping);
        }

    }

    return 0;
}