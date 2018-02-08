#include <iostream>
#include "eventsystem.h"
using namespace std;

class Client1 : public EventListener {
public:
    Client1() {
        EventSystem::instance().subscribe("EventForClass1", this);
    }
    void handleEvent(Event* event) {
        cout << "Class1 process event: " << event->eventId() << endl;
    }
};

class Client2 : public EventListener {
public:
    Client2() {
        EventSystem::instance().subscribe("EventForClass2", this);
    }

    void handleEvent(Event* event) {
        cout << "Class2 process event: " << event->eventId() << endl;
    }
};

//class ClientFactory {
//public:
//    Client1 newClient1(Manager* manager) {
//        return std::move(Client1);
//    }

//    Client2 newClient2(Manager* manager) {

//        return std::move(Client1);
//    }
//};

//class Manager {
//    void
//};



int main()
{
    Client1 c1;
    Client2 c2;

    EventSystem::instance().sendEvent("EventForClass1", nullptr, true);
    EventSystem::instance().sendEvent("EventForClass2");

    EventSystem::instance().processEvents();
    return 0;
}
