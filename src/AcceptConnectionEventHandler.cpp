#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>

#include "AcceptConnectionEventHandler.hpp"
#include "ServeRequestEventHandler.hpp"
#include "Reactor.hpp"

AcceptConnectionEventHandler::AcceptConnectionEventHandler(Reactor& reactor, int fd, const VirtualHostServer & virtualHostServer)
    : EventHandler(reactor, fd, virtualHostServer) {
}

AcceptConnectionEventHandler::AcceptConnectionEventHandler(const AcceptConnectionEventHandler & src)
    : EventHandler(src.reactor, src.fd, src.virtualHostServer) {
}

AcceptConnectionEventHandler::~AcceptConnectionEventHandler() {}

AcceptConnectionEventHandler& AcceptConnectionEventHandler::operator=(const AcceptConnectionEventHandler &rhs) {
	if (this != &rhs) {
        this->fd = rhs.fd;
        this->virtualHostServer = rhs.virtualHostServer;
    }

	return (*this);
}

void AcceptConnectionEventHandler::handleEvent() {
    unsigned int socketAddressSize;
    struct sockaddr_in socketAddress;

    socketAddress = this->virtualHostServer.getAddress();

    int newSocketfd = accept(fd, (sockaddr *)&socketAddress, &socketAddressSize);
    if (newSocketfd < 0)
    {
        throw std::runtime_error("Server failed to accept incoming connection from ADDRESS: ");// << inet_ntoa(socketAddress.sin_addr) << "; PORT: " << ntohs(socketAddress.sin_port));
    }

    std::cout << "Registering event(fd = " << newSocketfd << ") ServeRequestEventHandler" << std::endl;

    EventHandler *serveRequestEventHandler = new ServeRequestEventHandler(reactor, newSocketfd, this->virtualHostServer);
    reactor.registerEventHandler(newSocketfd, serveRequestEventHandler);
}
