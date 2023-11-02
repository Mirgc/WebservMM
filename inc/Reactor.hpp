#ifndef REACTOR_HPP
#define REACTOR_HPP

#include <map>
#include <utility>  // For std::pair
#include <cstring>

class EventHandler;

// Official patter sugests makeing this a singleton to avoid having multiple instances of Reactor
class Reactor {
public:
    Reactor();
    void runEventLoop();
    void stopEventLoop();
    void registerEventHandler(int fd, EventHandler* handler);
    void unregisterEventHandler(int fd);

private:
    // Holds all registered event handlers <SocketFD, EventHandler>
    std::map<int, EventHandler*> fdHandlerMap;
};

#endif
