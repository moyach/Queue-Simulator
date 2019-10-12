all: client server

client: client.cpp clientSocket.cpp message.cpp validator.cpp
	g++ -std=c++11 -o client client.cpp clientSocket.cpp message.cpp validator.cpp

server: server.cpp serverSocket.cpp message.cpp validator.cpp object.cpp service.cpp
	g++ -std=c++11 -o server server.cpp serverSocket.cpp message.cpp validator.cpp object.cpp service.cpp