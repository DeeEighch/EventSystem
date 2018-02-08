#ifndef LISTENER_H
#define LISTENER_H

#include "event.h"

class EventListener {
public:
    EventListener() {}
    virtual ~EventListener(){}
    virtual void handleEvent(Event* event) = 0;
};

#endif // LISTENER_H
