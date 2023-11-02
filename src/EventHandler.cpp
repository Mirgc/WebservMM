#include "EventHandler.hpp"
#include "Reactor.hpp"

EventHandler::EventHandler(Reactor& reactor, int fd):
    reactor(reactor), fd(fd) {
}

EventHandler::EventHandler(const EventHandler & src):
    reactor(src.reactor), fd(src.fd) {
}

EventHandler::~EventHandler() {}

EventHandler& EventHandler::operator=(const EventHandler &rhs) {
	if (this != &rhs) {
        this->reactor = rhs.reactor;
        this->fd = rhs.fd;
    }

	return (*this);
}
