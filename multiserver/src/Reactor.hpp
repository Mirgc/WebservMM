#ifndef REACTOR_HPP
#define REACTOR_HPP

#include <set>
#include <utility>  // For std::pair

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
    std::set<std::pair<int, EventHandler*> > fdHandlerPairs;
};

#endif
