#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

class Reactor;

class EventHandler {
public:
    EventHandler(Reactor& reactor, int fd);
    virtual ~EventHandler() {} 

    virtual void handleEvent() = 0;

protected:
    Reactor& reactor;
    int fd;
};

#endif
