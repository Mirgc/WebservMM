#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "AcceptConnectionEventHandler.hpp"
#include "ServeRequestEventHandler.hpp"
#include "Reactor.hpp"

AcceptConnectionEventHandler::AcceptConnectionEventHandler(Reactor& reactor, int fd)
    : EventHandler(reactor, fd) {
}

void AcceptConnectionEventHandler::handleEvent() {
    struct sockaddr_in socketAddress;
    unsigned int socketAddressSize;

    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = INADDR_ANY;
    // TODO: We need to connect hadnlers with Context (Server, Config, etc)
    socketAddress.sin_port = htons(8080);

    int newSocketfd = accept(fd, (sockaddr *)&socketAddress, &socketAddressSize);
    if (newSocketfd < 0)
    {
        throw std::runtime_error("Server failed to accept incoming connection from ADDRESS: ");// << inet_ntoa(socketAddress.sin_addr) << "; PORT: " << ntohs(socketAddress.sin_port));
    }

    EventHandler *serveRequestEventHandler = new ServeRequestEventHandler(reactor, newSocketfd);
    reactor.registerEventHandler(newSocketfd, serveRequestEventHandler);
}
