all: client server

client: client.cpp clientSocket.cpp message.cpp validator.cpp
	g++ -o client client.cpp clientSocket.cpp message.cpp validator.cpp

server: server.cpp serverSocket.cpp message.cpp validator.cpp object.cpp
	g++ -o server server.cpp serverSocket.cpp message.cpp validator.cpp object.cpp