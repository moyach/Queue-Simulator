
#include <iostream>
#include "serverSocket.h"
#include "validator.h"
#include "message.h"
#include <string.h>
#include <thread>

#include <list>
#include <queue>

using namespace std;



ServerSocket s;
bool running = false;
int numServices;
string currentCommand;

list<thread> serviceThreads;






// int main(int argc, char **argv) {

//   fd_set fds, readfds;
//   int i, clientaddrlen;
//   int clientsock[2], rc, numsocks = 0, maxsocks = 2;


//   //Creation of socket that uses TCP
//   int serversock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//   int activated = 1;

//   setsockopt(serversock, SOL_SOCKET, SO_REUSEADDR, &activated, sizeof(activated)); //es para que si se apaga el server, pueda levantar sin esperar

//   if (serversock == -1) perror("Socket");

//   struct sockaddr_in serveraddr, clientaddr;  
//   bzero(&serveraddr, sizeof(struct sockaddr_in));
//   serveraddr.sin_family = AF_INET;
//   serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
//   serveraddr.sin_port = htons(6782);

//   if (bind(serversock, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in)) != 0) {
//      perror("Bind");
//      return -1;
//   } 
   

//   if (listen(serversock, SOMAXCONN) == -1)
//     perror("Listen");

//   FD_ZERO(&fds);
//   FD_SET(serversock, &fds);

//   while(1) {

//     readfds = fds;
//     rc = select(FD_SETSIZE, &readfds, NULL, NULL, NULL);

//     if (rc == -1) {
//       perror("Select");
//       break;
//     }

//     for (i = 0; i < FD_SETSIZE; i++) {
//       if (FD_ISSET(i, &readfds)) {
//         if (i == serversock) {
//           if (numsocks < maxsocks) {
//             clientsock[numsocks] = accept(serversock,
//                                       (struct sockaddr *) &clientaddr,
//                                       (socklen_t *)&clientaddrlen);
                              
//             if (clientsock[numsocks] == -1) perror("Accept");
//             FD_SET(clientsock[numsocks], &fds);
//             numsocks++;
//           } else {
//             printf("Ran out of socket space.\n");

//           }
//         } else {
//           int messageLength = 5;
//           char message[messageLength+1];
//           int in, index = 0, limit = messageLength+1;

        
//           while ((in = recv(i, &message[index], limit, 0)) > 0) {
//             std::cout << "recieved something from " << index << " and is: " << message << std::endl;
//             return 0;

//             index += in;
//             limit -= in;
//           }

//           printf("%d\n", index);
//           printf("%s\n", message);

//         }
//       }
//     }
//   }

//   close(serversock);
//   return 0;
// }

void manageTraffic() {
  string data;
  while (running) {
    data = s.Listen();
    if (data.length() > 0) {
      if (data[0] == 'O') {
        data.erase(0, 1);
        cout << "I have execution of: " << data << endl;
      }
    }
    
  }
}

void attendQueues() {

}

int main(int argc, char **argv) {
  if (argc == 2) {
    string strServices = string(argv[1]);
    if (Validator::isNumber(strServices)) {
      numServices = atoi(strServices.c_str());
      if (numServices == 0) {
        cout << Message::errorFormat("Number of services", "0");
        return 0;
      } else {
        if (s.Connect()) {
          running = true;
          cout << Message::newlineMessage("Server is up and running");

          for (int i  = 0; i < numServices; i++) {
            serviceThreads.push_back(thread(attendQueues));
          }

          thread trafficManager (manageTraffic);

          while (running) {
            currentCommand.clear();
            getline(cin, currentCommand);
            if (currentCommand.compare("e") == 0) {
              running = false;
            }
          }
          cout << Message::newlineMessage("Waiting for server manager to end...");
          trafficManager.join();
          s.Close();

        }
      }
    }
  } else {
    cout << Message::newlineMessage("Must only specify number of services as parameter");
  }
  return 0;
}
