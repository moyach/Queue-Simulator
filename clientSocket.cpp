#include "clientSocket.h"


bool ClientSocket::StartConnection() {
    this->clientSoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->clientSoc == -1) {
        perror("Socket");
        return false;
    }
    bzero((void *) &this->serverAddr, sizeof(this->serverAddr));
    this->serverAddr.sin_family = AF_INET;
    this->serverAddr.sin_port = htons(6782);
    this->serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(this->clientSoc, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
    perror("Connect");
    return false;
  }
  return true;
}


void ClientSocket::Send(std::string message) {


    send(this->clientSoc, message.c_str(), message.length(), 0);

}

void ClientSocket::Disconnect() {
    close(this->clientSoc);
}