#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP
#include <netinet/in.h>

class Reactor;

class EventHandler {
public:
    EventHandler(Reactor& reactor, int fd, struct sockaddr_in socketAddress);
    EventHandler(const EventHandler & src);
    virtual ~EventHandler();

    EventHandler & operator=(EventHandler const & rhs);

    virtual void handleEvent() = 0;

protected:
    Reactor& reactor;
    int fd;
    struct sockaddr_in socketAddress;
};

#endif
