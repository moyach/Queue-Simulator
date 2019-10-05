#ifndef SERVERSOCKET_H

#define SERVERSOCKET_H

#define MAXCLIENTS 10
#define BUFFLEN 1024

#include <stdio.h>
#include <string.h> //strlen
#include <string>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO


class ServerSocket {
    private:

        //server socket
        int serverSock;

        //new client connection socket
        int newSock;

        int addrlen;

        //check for activity from any of the sockets
        int activity;

         //set of socket descriptors to monitor
        fd_set readFds;


        struct sockaddr_in serveraddr;
        struct sockaddr_in clientaddr;

        int clientaddLen;


        int clientSocks[MAXCLIENTS], numSocks;

    public:
        ServerSocket();
        bool Connect();
        std::string Listen();

        void Close();

};

#endif