#ifndef EVENT_H
#define EVENT_H

#include <string>
using std::string;
#define EVENT_ID_TYPE string


class Event {
    EVENT_ID_TYPE m_eventId;
    void* m_parameter;
    bool m_isPermanent;
public:
    Event(EVENT_ID_TYPE eventId, void* parameter = 0, bool isPermanent = false) {
        this->m_eventId = eventId;
        this->m_parameter = parameter;
        this->m_isPermanent = isPermanent;
    }

    ~Event() {}

    EVENT_ID_TYPE eventId() const {return m_eventId; }
    void* parameter() {return m_parameter; }
    bool isPermanent() {return m_isPermanent; }

};
#endif // EVENT_H
