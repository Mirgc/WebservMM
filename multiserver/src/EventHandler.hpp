#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

class Reactor;

class EventHandler {
public:
    EventHandler(Reactor& reactor, int fd);
    void handleEvent();

private:
    Reactor& reactor;
    int fd;
};

#endif
