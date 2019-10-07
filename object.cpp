#include "object.h"


Object::Object(int pExecTime, int qNumber) {
    execTime = pExecTime;
    time(&arrival);
    queueNumber = qNumber;
}