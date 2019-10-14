
#include <iostream>
#include <string.h>
#include <thread>
#include <mutex>
#include <stdlib.h>

#include <list>
#include <queue>

#include "object.h"
#include "serverSocket.h"
#include "validator.h"
#include "message.h"
#include "service.h"

using namespace std;



ServerSocket s;
bool running = false;
bool report = true;
int numServices;
int numQueues;
string currentCommand;

string finalTable = "";

vector<thread> serviceThreads;

list<queue<Object> > listQueues;

mutex m;





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

int determineQueueNumber() {
  //check if there is a queue with the least amount
  int index = -1;
  int aux = -1;
  int amount = -1;


  list<queue<Object> > ::iterator it;
  for (it = listQueues.begin(); it != listQueues.end(); ++it) {
    aux++;
    
    queue<Object> currentQueue = (queue<Object>)* (it);
    if (amount == -1) {
      amount = currentQueue.size();
      index = aux;

    } else if (amount != -1 && currentQueue.size() < amount) {
      amount = currentQueue.size();
      index = aux;
    }
  }
  return index;
}

void insertIntoQueue(int queueNumber, Object o) {
  
  list<queue<Object> > ::iterator it;
  int index = -1;
  for (it = listQueues.begin(); it != listQueues.end(); ++it) {
    index++;
    if (index == queueNumber) {
      //then assign
      it->push(o);
    }
  }
}

void manageTraffic() {
  string data;
  while (running) {
    data = s.Listen();
    if (data.length() > 0) {
      if (data[0] == 'O') {
        data.erase(0, 1);
        time_t current;
        time(&current);

        int execTime = atoi(data.c_str());

        int queueNumber = determineQueueNumber();
        Object obj(execTime, queueNumber);
        insertIntoQueue(queueNumber, obj);
        cout << "Adding object with execution of: " << execTime << " to line number: " << queueNumber << endl;
        
      }
    }
  }
}

bool validUserData(list<string> params) {
  bool valid = true;

  string strServices = params.front();
  params.pop_front();
  string strQueues = params.front();

  if (!Validator::isNumber(strServices)) {
      cout << Message::newlineMessage(Message::errorFormat("Number of services", "anything other than numbers"));
      valid = false;;

  }else if (!Validator::isNumber(strQueues)) {
      cout << Message::newlineMessage(Message::errorFormat("Number of queues", "anything other than numbers"));
      valid = false;
  } else {
    numServices = atoi(strServices.c_str());
    numQueues = atoi(strQueues.c_str());

    if (numServices == 0) {
      cout << Message::newlineMessage(Message::errorFormat("Number of services", " only 0"));
        valid = false;
    } else if (numQueues == 0) {
      cout << Message::newlineMessage(Message::errorFormat("Number of queues", "only 0"));
        valid = false;
    }
  } 
    return valid;
}

void setUpQueues(int numQueues) {
  do {
    queue<Object> newQueue;
    listQueues.push_back(newQueue);
    numQueues--;
  } while (numQueues > 0);
}

Object getQueueObjectAt(int index) {
  list<queue<Object> > ::iterator it;
  int currentIndex = 0;
  for (it = listQueues.begin(); it != listQueues.end(); ++it) {
    if (currentIndex == index) {
      //use current queue
      if (it->size() == 0) {
        return Object(-1, -1);
      } else {
        Object o = it->front();
        it->pop();
        return o;
      }
    } else {
      currentIndex++;
    }
  }
  //return an empty object
  return Object(-1, -1);
}

void attendQueue(const int serviceNumber) {
  Service s(serviceNumber);

  bool randomQueue = false;
  int index;
  
  list<queue<Object> > ::iterator it;

  string message = "Service number: ";
  message += to_string(serviceNumber);
  message += " is up for service";

  m.lock();
  
  cout << Message::newlineMessage(message);
  m.unlock();
  
  if (numServices != numQueues) {
    randomQueue = true;
  } else {
    index = serviceNumber;
  }

  while (running) {
    if (randomQueue) {
      index = rand() % numQueues;
    }

    if (currentCommand.compare("r") == 0) {
      cout << s.ToString();
    }

    sleep(1);
    Object o = getQueueObjectAt(index);

    if (o.getQueueNumber() != -1) {
      //it is a valid object from queue
      message.clear();
      message += to_string(serviceNumber);
      message += " is rtrying to recieve an object from queue number: ";
      message += to_string(index);

      m.lock();
      cout << Message::newlineMessage(message);
      m.unlock();

      //o.setAttentTime();
      int executionTime = o.getExecution();
      sleep(executionTime);
      s.finishAttention(executionTime);

      cout << "Client at: " << serviceNumber << " has finished its time at the service" << endl;
      //showStats of object leaving
    }
  }
  finalTable = finalTable + s.ToString();
  //cout << s.ToString();
}

void setUpServices(int numServices) {
  for (int i = 0; i < numServices; i++) {
    serviceThreads.push_back(thread(attendQueue, i));
  }
}

void showQueues() {
  cout << Message::newlineMessage("Queue#\t|\t Time of arrival \t\t|\t ExecTime \t|\t WaitingTime");
  list<queue<Object> > ::iterator it;
  for (it = listQueues.begin(); it != listQueues.end(); ++it) {
    queue<Object> temp = (queue<Object>)* (it);
    int size = temp.size();
    for (int i = 0; i < size; i++) {
      Object o = temp.front();
      temp.pop();
      cout << o.ToString();
      temp.push(o);
    }
  }
}



int main(int argc, char **argv) {
  if (argc == 3) {
    string strServices = string(argv[1]);
    string strQueues = string(argv[2]);
    list<string> input;
    input.push_back(strServices);
    input.push_back(strQueues);

    if (validUserData(input)) {
      if (s.Connect()) {
        running = true;
        cout << Message::newlineMessage("Server is up and running");

        thread trafficManager (manageTraffic);
        setUpQueues(numQueues);
        setUpServices(numServices);

        while (running) {
          //currentCommand.clear();
          getline(cin, currentCommand);
          if (currentCommand.compare("e") == 0) {
            running = false;
          } else if (currentCommand.compare("q") == 0) {
            showQueues();
          }
        }

      //close manager listener thread
        cout << Message::newlineMessage("Waiting for server manager to end...");
        
        trafficManager.join();
        s.Close();

        //close service threads
        for (int i = 0; i < numServices; i++) {
          serviceThreads[i].join();
        }

        system("clear");
        cout << Message::newlineMessage("ID\t|\t #ObjsAttended \t\t|\t TotalExecution \t|\t AVGTime/Obj");
        cout << finalTable;
      }
    }
  } else {
    cout << Message::newlineMessage("Whoops, you have to spcecify only the number of services and number of queues");
  }
  return 0;
}

