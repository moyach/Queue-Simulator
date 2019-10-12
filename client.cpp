#include "client.h"

using namespace std;



bool running = false;
int minimumExc, maximumExc, creationRate;
int operationMode = -1; //0 manual, 1 fileReading

ClientSocket c;

void showClientMenuOptions() {
  cout << Message::newlineMessage("=====Client Menu=====");
  cout << Message::newlineMessage("1. Generate random objects");
  cout << Message::newlineMessage("2. Exit");

  std::string option;
  cout << Message::inlineMessage("Enter the option number:");
  getline(std::cin, option);
  handleOption(option);
}

void handleOption(std::string option) {
  if (option.compare("1") == 0 && !running) {

    list<string> input = askInputForObjectCreation();
    if (validInput(input)) {
      assignValues(input);
      operationMode = 0;
      running = true;

    } else {
      showClientMenuOptions();
    }

  } else if (option.compare("2") == 0 && !running) {
    return;

  } else if (option.compare("e") == 0 && running) {

    running = false;

  } else if (!running){
      cout << Message::newlineMessage(Message::invalidOption);
      showClientMenuOptions();
  }
}

list<string> askInputForObjectCreation() {
  string minExecTime, maxExecTime, creationRate;
  list<string> input;

  cout << Message::inlineMessage("Insert the amount (seconds) of minimum execution time:");
  getline(std::cin, minExecTime);
  cout << Message::inlineMessage("Insert the amount (seconds) of maximum execution time:");
  getline(std::cin, maxExecTime);
  cout << Message::inlineMessage("Insert the amount (seconds) of time it takes to create a new object:");
  getline(std::cin, creationRate);

  input.push_back(minExecTime);
  input.push_back(maxExecTime);
  input.push_back(creationRate);
  
  return input;
}

list<string> askInputForFileReading() {
  string fileName;
  list<string> input;

  cout << Message::inlineMessage("Insert the filename:");
  getline(std::cin, fileName);

  input.push_back(fileName);

  return input;
}

bool validInput(list<string> input) {
  bool valid = true;

  if (input.size() == 1) {
    //it is a fileName
    string fileName = input.front();
    if (fileName.compare("") == 0) {
      cout << Message::errorEmpty("Filename");
      valid = false;
    } else {
      return true;
    }

  } else if (input.size() == 3) {
    //object with min, max and creationRate
    string minExc = input.front();
    input.pop_front();
    string maxExc = input.front();
    input.pop_front();
    string creationRate = input.front();

    if (minExc.compare("") == 0 || maxExc.compare("") == 0 || creationRate.compare("") == 0) {
      cout << Message::errorEmpty("no input");
      valid = false;
    } else if (!Validator::isNumber(minExc) || !Validator::isNumber(maxExc) || !Validator::isNumber(creationRate)) {
      cout << Message::errorFormat("no input", "anything other than numbers");
      valid = false;
    } else {
      int minExcInt = atoi(minExc.c_str());
      int maxExcInt = atoi(maxExc.c_str());
      int creationRateInt = atoi(creationRate.c_str());
      if (minExcInt > maxExcInt) {
        cout << Message::error("minimum cannot be greater than maximum");
        valid = false;

      } else if (maxExcInt == 0) {
        cout << Message::errorFormat("maximum execution rate", "only 0");
        valid = false;
      } else if (minExcInt == 0) {
        cout << Message::errorFormat("minimum execution rate", "only 0");
      }
    }
  } else {
    valid = false;
  }
  return valid;
}

void assignValues(list<string> input) {
  minimumExc = atoi(input.front().c_str());
  input.pop_front();
  maximumExc = atoi(input.front().c_str());
  input.pop_front();
  creationRate = atoi(input.front().c_str());
}

void createObjects() {
  while (running) {
    int randomExecution = minimumExc + rand() % (maximumExc + 1 - minimumExc);
    
    char object[] = "";
    cout << Message::newlineMessage("Sending an object with execution of: " + to_string(randomExecution));
    sprintf(object, "%s%d","O",randomExecution);
    c.Send(object);
    
    sleep(creationRate);
  }
}

int main(int argc, char **argv) {
  srand(time(NULL));



  if (c.StartConnection()) {

    cout << Message::newlineMessage("Connected successfully to server");
    showClientMenuOptions();

    if (operationMode == 0) {
      thread objectCreator (createObjects);
      std::string input;

      while (running) {
        input.clear();
        getline(std::cin, input);
        handleOption(input);
      }
      cout << Message::newlineMessage("Waiting for object spawner to end...");
      objectCreator.join();
      c.Disconnect();
    }
  }

  return 0;
  

  

  // struct sockaddr_in servaddr;
  // int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  //
  // if (sock == -1) perror("Socket");
  //
  // bzero((void *) &servaddr, sizeof(servaddr));
  // servaddr.sin_family = AF_INET;
  // servaddr.sin_port = htons(6782);
  // servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  //
  // if (-1 == connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)))
  //   perror("Connect");
  //
  // while(1) {
  //
  //   char message[6];
  //   fgets(message, 6, stdin);
  //
  //   message[5] = '\0';
  //
  //   send(sock, message, 6, 0);
  // }
  //
  //
  // close(sock);
}
