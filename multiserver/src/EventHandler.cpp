#include "EventHandler.hpp"
#include "Reactor.hpp"

EventHandler::EventHandler(Reactor& reactor, int fd):
    reactor(reactor), fd(fd) {
}
