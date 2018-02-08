#include "eventsystem.h"

EventSystem::EventSystem()
{
}

EventSystem::~EventSystem() {this->exit();}

EventSystem::EventSystem(const EventSystem &) {}

EventSystem &EventSystem::operator=(const EventSystem &) {}

bool EventSystem::alreadyRegistered(EVENT_ID_TYPE eventId, EventListener *listener)
{
    bool registered = false;

    pair<multimap<EVENT_ID_TYPE, EventListener*>::iterator,
            multimap<EVENT_ID_TYPE, EventListener*>::iterator> range;

    range = database.equal_range(eventId);

    for (multimap<EVENT_ID_TYPE, EventListener*>::iterator iter = range.first;
         iter != range.second; iter++) {

        if ((*iter).second == listener) {
            registered = true;
            break;
        }

    }
    return registered;
}

void EventSystem::dispatchEvent(Event *event)
{
    pair<multimap<EVENT_ID_TYPE, EventListener*>::iterator,
            multimap<EVENT_ID_TYPE, EventListener*>::iterator> range;
    range = database.equal_range(event->eventId());

    for (multimap<EVENT_ID_TYPE, EventListener*>::iterator iter = range.first;
         iter != range.second; iter++) {
        (*iter).second->handleEvent(event);
    }
}

EventSystem &EventSystem::instance()
{
    static EventSystem instance;
    return instance;
}

void EventSystem::subscribe(EVENT_ID_TYPE eventId, EventListener *listener)
{
    if (!listener || alreadyRegistered(eventId, listener)) {
        return;
    }
    database.insert(std::make_pair(eventId, listener));
}

void EventSystem::unsubscribe(EVENT_ID_TYPE eventId, EventListener *listener)
{
    pair<multimap<EVENT_ID_TYPE, EventListener*>::iterator,
            multimap<EVENT_ID_TYPE, EventListener*>::iterator> range;

    range = database.equal_range(eventId);

    for (multimap<EVENT_ID_TYPE, EventListener*>::iterator iter = range.first;
         iter != range.second; iter ++) {
        if ((*iter).second == listener) {
            iter = database.erase(iter);
            break;
        }
    }
}

void EventSystem::unsubscribeAll(EventListener *listener)
{
    multimap<EVENT_ID_TYPE, EventListener*>::iterator iter = database.begin();

    while (iter != database.end()) {
        if ((*iter).second == listener) {
            iter = database.erase(iter);
        } else {
            iter++;
        }
    }
}

void EventSystem::sendEvent(EVENT_ID_TYPE eventId, void *data, bool isPermanent)
{
    Event newEvent(eventId, data, isPermanent);
    currentEvents.push_back(newEvent);
    can_process.notify_one();

}

#include <iostream>
void EventSystem::processEvents()
{
    std::unique_lock<std::mutex> lock(mtx);
    while (currentEvents.size()) {

        if (currentEvents.size() == 1) {
            can_process.wait(lock, [&]{return !currentEvents.front().isPermanent();});
        }

        dispatchEvent(&currentEvents.front());
        if (currentEvents.front().isPermanent()) {
            //Event e = currentEvents.front();
            currentEvents.push_back(currentEvents.front());
            currentEvents.pop_front();

        } else {
            currentEvents.pop_front();
        }

        std::cout << "size" << currentEvents.size() << std::endl;
    }
}

void EventSystem::clearEvents()
{
    currentEvents.clear();
}

void EventSystem::exit()
{
    currentEvents.clear();
    database.clear();

}

