#ifndef ACCEPT_CONNECTION_EVENT_HANDLER_HPP
#define ACCEPT_CONNECTION_EVENT_HANDLER_HPP

class Reactor;

#include "EventHandler.hpp"

class AcceptConnectionEventHandler: public EventHandler {
public:
    AcceptConnectionEventHandler(Reactor& reactor, int fd);
    virtual void handleEvent();
};

#endif
