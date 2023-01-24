# Server-Client Bomberman

Bomberman clone with network multiplayer 

![Bomb](https://user-images.githubusercontent.com/47029889/214389747-ce353976-b9c4-446e-91b8-851638622306.gif)

Used libraries: 
- Charlie from teacher - got further extended as needed,
- pugixml from the net

UDP Network; Server and client based
The server is authoritative 

Project plan got approved

Four players are the maximum
You can win and lose the game

Controls:
WASD - Moving
Space - Planting bomb

In menu/In Finding Server:
W - up
S - down
Enter - confirm

In Lobby:
Backspace - back
W - up
S - down
Enter - confirm

Server Discovery possible
Inputinator = PositionInputination
Interpolator = PositionInterpolator
Reliable Messages, or hints of it, 
exist in the server_playing and at the bottom of the charlie_messages

Send rate of the server is 20 packets per seconds and simulates 60 ticks

Network Overlay exists with the specified variables

Timeouts get handled nicely

Information about map download and bandwith cut can be found in the charlie_network.hpp


