#include "object.h"



Object::Object(int pExecTime, int qNumber) {
    execTime = pExecTime;
    time(&arrival);
    queueNumber = qNumber;
}

int Object::getQueueNumber() {
    return queueNumber;
}

int Object::getExecution() {
    return execTime;
}

void Object::setAttentTime() {
    time(&attention);
}

std::string Object::ToString() {

    std::string s;

    s += std::to_string(queueNumber) + "\t|\t";
    char* t = ctime(&arrival);
    if (t[strlen(t)  - 1] == '\n') {
        t[strlen(t) - 1] = '\0';
        s += t;
        s += "\t|\t";
    }
    s += std::to_string(execTime) + "\t\t|\t";

    time_t current;
    time(&current);
    double waitingTime = difftime(current, arrival);
    s += std::to_string(waitingTime);
    s += "\n";
    return s;
}