#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include <list>
#include <map>
#include <mutex>
#include <condition_variable>

using std::multimap;
using std::list;
using std::pair;

#include "event.h"
#include "listener.h"

class EventSystem
{
    multimap<EVENT_ID_TYPE, EventListener*> database;
    list<Event> currentEvents;
    std::mutex mtx;
    std::condition_variable can_process;

    EventSystem();
    ~EventSystem();
    EventSystem(const EventSystem& other);
    EventSystem& operator=(const EventSystem&);

    bool alreadyRegistered(EVENT_ID_TYPE eventId, EventListener* listener);
    void dispatchEvent(Event* event);
public:
    static EventSystem& instance();

    void subscribe(EVENT_ID_TYPE eventId, EventListener* listener);
    void unsubscribe(EVENT_ID_TYPE eventId, EventListener* listener);
    void unsubscribeAll(EventListener* listener);
    void sendEvent(EVENT_ID_TYPE eventId, void* data = nullptr, bool isPermanent = false);
    void processEvents();
    void clearEvents();
    void exit();




};

#endif // EVENTSYSTEM_H
