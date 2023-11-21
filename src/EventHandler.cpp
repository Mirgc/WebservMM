#include "EventHandler.hpp"
#include "Reactor.hpp"

EventHandler::EventHandler(Reactor& reactor, int fd, struct sockaddr_in socketAddress):
    reactor(reactor), fd(fd), socketAddress(socketAddress){
}

EventHandler::EventHandler(const EventHandler & src):
    reactor(src.reactor), fd(src.fd) , socketAddress(src.socketAddress){
}

EventHandler::~EventHandler() {}

EventHandler& EventHandler::operator=(const EventHandler &rhs) {
	if (this != &rhs) {
        this->fd = rhs.fd;
        this->socketAddress = rhs.socketAddress;
    }

	return (*this);
}
