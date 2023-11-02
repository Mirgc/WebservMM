#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>

#include "AcceptConnectionEventHandler.hpp"
#include "ServeRequestEventHandler.hpp"
#include "Reactor.hpp"

AcceptConnectionEventHandler::~AcceptConnectionEventHandler() {}

AcceptConnectionEventHandler::AcceptConnectionEventHandler(Reactor& reactor, int fd)
    : EventHandler(reactor, fd) {
}

void AcceptConnectionEventHandler::handleEvent() {
    struct sockaddr_in socketAddress;
    unsigned int socketAddressSize;

    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = INADDR_ANY;
    // TODO: We need to connect handlers with Context (Server, Config, etc)?
    // Will different event handlers have different extra properties to set up?
    // At least we need the VirtualHostServer port here
    socketAddress.sin_port = htons(8080);

    int newSocketfd = accept(fd, (sockaddr *)&socketAddress, &socketAddressSize);
    if (newSocketfd < 0)
    {
        throw std::runtime_error("Server failed to accept incoming connection from ADDRESS: ");// << inet_ntoa(socketAddress.sin_addr) << "; PORT: " << ntohs(socketAddress.sin_port));
    }

    std::cout << "Registering event(fd = " << newSocketfd << ")...ServeRequestEventHandler" << std::endl;

    EventHandler *serveRequestEventHandler = new ServeRequestEventHandler(reactor, newSocketfd);
    reactor.registerEventHandler(newSocketfd, serveRequestEventHandler);
}
