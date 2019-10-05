#include "serverSocket.h"
#include "message.h"
#include <iostream>


ServerSocket::ServerSocket() {
    //initialize all client sockets to 0
    for (int i  = 0; i < MAXCLIENTS; i++) {
        clientSocks[i] = 0;
        numSocks = 0;
    }
}


bool ServerSocket::Connect() {

    //create the socket
    serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int activated = 1;

    //setsockopt to be able to turn on the server without having to wait if turned off
    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &activated, sizeof(activated));
    if (serverSock == -1) {
        perror("Socket");
        return false;
    }

    //set socket type
    bzero(&serveraddr, sizeof(struct sockaddr_in));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(6782);

    //bind socket
    if (bind(serverSock, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in)) != 0) {
        perror("Bind");
        return false;
    } 

    //try to specify max connections for server socket
    if (listen(serverSock, MAXCLIENTS) == -1) {
        perror("listen");
        return false;
    }

    return true;
}

std::string  ServerSocket::Listen() {

    std::string data = "";

    //clear connections, then add serverSocket
    FD_ZERO(&readFds);
    FD_SET(serverSock, &readFds);

    //add server socket to set
    FD_SET(serverSock, &readFds);

    //add children
    for (int i = 0; i < MAXCLIENTS; i++) {
        int socketDescriptor = clientSocks[i];

        //if the socket descriptor is valid, then we add
        if (socketDescriptor > 0) {
            FD_SET(socketDescriptor, &readFds);
        }
    }

    //wait for an activity, timeout is null so it just waits
    activity = select(FD_SETSIZE, &readFds, NULL, NULL, NULL);
    if (activity == -1) {
        perror("select");
    }

    if (FD_ISSET(serverSock, &readFds)) {
        //if something happened on server socket, then it is an incoming connection
        if (numSocks < MAXCLIENTS) {
            newSock = accept(serverSock, (struct sockaddr *) &clientaddr, (socklen_t *)&clientaddLen);
            if (newSock == -1) {
                perror("Accept");
            } else {
                printf("New connection socket fd is %d, ip is %s, port is %d\n", newSock, inet_ntoa(serveraddr.sin_addr), ntohs(serveraddr.sin_port));

                //add new connection
                for (int i = 0; i < MAXCLIENTS; i++) {
                    if (clientSocks[i] == 0) {
                        clientSocks[i] = newSock;
                        numSocks++;
                        break;
                    }
                }
            }
        } else {
            std::cout << "I have " << numSocks << std::endl;
            std::cout << Message::newlineMessage("Ran out of socket space");
        }
        //it is something that happened in another socket
    } else {
        char buffer[BUFFLEN];
        for (int i = 0; i < MAXCLIENTS; i++) {
            if (FD_ISSET(clientSocks[i], &readFds)) {
                int recievedBytes = read(clientSocks[i], buffer, BUFFLEN);
                if (recievedBytes == 0) {
                    //somebody disconnected
                    getpeername(clientSocks[i], (struct sockaddr *)&serveraddr, (socklen_t *) sizeof(serveraddr));
                    printf("Host disconnected , ip %s , port %d \n", inet_ntoa(serveraddr.sin_addr) , ntohs(serveraddr.sin_port));

                    //close socket
                    close(clientSocks[i]);
                    clientSocks[i] = 0;
                    numSocks--;
                } else {
                    buffer[recievedBytes] = '\0';
                    data = std::string(buffer);
                }

            }
        }
    
    }
    return data;
}

void ServerSocket::Close() {
    for (int i  = 0; i < MAXCLIENTS; i++) {
        if (clientSocks[i] != 0) {
            close(clientSocks[i]);
            clientSocks[i] = 0;
        }
    }
    close(serverSock);
}