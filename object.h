#ifndef OBJECT_H
#define OBJECT_H
#include <time.h>

class Object {
    private:
        int queueNumber;
        int execTime;
        time_t arrival;

    public:
        Object(int pExecTime, int qNumber);

};

#endif