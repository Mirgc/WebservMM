#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

class Reactor;

class IEventHandler {
public:
    IEventHandler(Reactor& reactor, int fd);
    virtual ~IEventHandler() {} 

    virtual void handleEvent() = 0;

private:
    Reactor& reactor;
    int fd;
};

#endif
