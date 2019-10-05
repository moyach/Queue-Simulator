#ifndef CLIENT_H
#define CLIENT_H


#include <thread> //thread creation

#include <iostream> //input output stream manipulation

#include <list> //stl list container

#include <string> //use string data type

#include <fstream> //file IO
#include <sstream>

#include <stdio.h>
#include <stdlib.h> //itoa

#include "validator.h"
#include "message.h"
#include "clientSocket.h"

using namespace std;


void showClientMenuOptions();

void handleOption(string option);

list<string> askInputForObjectCreation();

list<string> askInputForFileReading();

bool validInput(list<string> input);

void assignValues(list<string> input);

void createObjects();


#endif
