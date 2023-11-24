#include "EventHandler.hpp"
#include "Reactor.hpp"

EventHandler::EventHandler(Reactor& reactor, int fd, const VirtualHostServer & virtualHostServer, struct sockaddr_in socketAddress):
    reactor(reactor), fd(fd), virtualHostServer(virtualHostServer), socketAddress(socketAddress) {
}

EventHandler::EventHandler(const EventHandler & src):
    reactor(src.reactor), fd(src.fd), virtualHostServer(src.virtualHostServer), socketAddress(src.socketAddress){
}

EventHandler::~EventHandler() {}

EventHandler& EventHandler::operator=(const EventHandler &rhs) {
	if (this != &rhs) {
        this->fd = rhs.fd;
        this->socketAddress = rhs.socketAddress;
    }

	return (*this);
}
