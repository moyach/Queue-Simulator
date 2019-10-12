#ifndef OBJECT_H
#define OBJECT_H
#include <time.h>
#include <string>
#include <string.h>
#include <sstream>

class Object {
    private:
        int queueNumber;
        int execTime;
        time_t arrival;
        time_t attention;

    public:
        Object(int pExecTime, int qNumber);
        int getQueueNumber();
        int getExecution();
        void setAttentTime();
        std::string ToString();

};

#endif