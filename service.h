#ifndef SERVICE_H

#define SERVICE_H
#include <time.h>
#include <string>
#include <string.h>

class Service {
    private:
        int ID;
        time_t initialTime;
        int numClientsAttended;
        int totalExecutions;

    public:
        Service(int pId);
        void finishAttention(int execution);
        std::string ToString();
        int getId();
};
#endif