#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>

#include "AcceptConnectionEventHandler.hpp"
#include "ServeRequestEventHandler.hpp"
#include "Reactor.hpp"

AcceptConnectionEventHandler::AcceptConnectionEventHandler(Reactor& reactor, int fd, struct sockaddr_in socketAddress)
    : EventHandler(reactor, fd, socketAddress) {
}

AcceptConnectionEventHandler::AcceptConnectionEventHandler(const AcceptConnectionEventHandler & src)
    : EventHandler(src.reactor, src.fd, src.socketAddress) {
}

AcceptConnectionEventHandler::~AcceptConnectionEventHandler() {}

AcceptConnectionEventHandler& AcceptConnectionEventHandler::operator=(const AcceptConnectionEventHandler &rhs) {
	if (this != &rhs) {
        this->fd = rhs.fd;
        this->socketAddress = rhs.socketAddress;
    }

	return (*this);
}


void AcceptConnectionEventHandler::handleEvent() {
    unsigned int socketAddressSize;

    // TODO: We need to connect handlers with Context (Server, Config, etc)?
    // Will different event handlers have different extra properties to set up?
    // At least we need the VirtualHostServer port here

    int newSocketfd = accept(fd, (sockaddr *)&socketAddress, &socketAddressSize);
    if (newSocketfd < 0)
    {
        throw std::runtime_error("Server failed to accept incoming connection from ADDRESS: ");// << inet_ntoa(socketAddress.sin_addr) << "; PORT: " << ntohs(socketAddress.sin_port));
    }

    std::cout << "Registering event(fd = " << newSocketfd << ")...ServeRequestEventHandler" << std::endl;

    EventHandler *serveRequestEventHandler = new ServeRequestEventHandler(reactor, newSocketfd, socketAddress);
    reactor.registerEventHandler(newSocketfd, serveRequestEventHandler);
}
