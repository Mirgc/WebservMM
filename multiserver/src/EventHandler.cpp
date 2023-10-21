#include "EventHandler.hpp"
#include "Reactor.hpp"

IEventHandler::IEventHandler(Reactor& reactor, int fd):
    reactor(reactor), fd(fd) {
}
