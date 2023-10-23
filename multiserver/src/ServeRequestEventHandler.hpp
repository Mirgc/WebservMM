#ifndef SERVER_REQUEST_EVENT_HANDLER_HPP
#define SERVER_REQUEST_EVENT_HANDLER_HPP

class Reactor;

#include "EventHandler.hpp"

class ServeRequestEventHandler: public EventHandler {
public:
    ServeRequestEventHandler(Reactor& reactor, int fd);
    virtual void handleEvent();
};

#endif
