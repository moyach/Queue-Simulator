#include "service.h"



Service::Service(int pId) {
    ID = pId;
    time(&initialTime);
}

void Service::finishAttention(int execution) {
    numClientsAttended++;
    totalExecutions = totalExecutions + execution;
}

std::string Service::ToString() {
    std::string s;
    s += std::to_string(ID) + "\t|\t\t";
    s += std::to_string(numClientsAttended) + "\t\t|\t\t";
    s += std::to_string(totalExecutions) + "\t\t|\t";

    time_t current;
    time(&current);
    double average = 0;
    
    if (numClientsAttended > 0) {
        average = totalExecutions / numClientsAttended;

    }
    s += std::to_string(average);
    s += "\n";

    return s;
}

int Service::getId() {
    return ID;
}