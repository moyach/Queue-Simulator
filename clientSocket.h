#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <errno.h>
#include <unistd.h> //close, sleep
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h> //strlen


#include <list>
#include <string>


class ClientSocket {
    private:
        struct sockaddr_in serverAddr;
        int clientSoc;

    public:
        bool StartConnection();
        void Send(std::string message);
        void Disconnect();
};


#endif