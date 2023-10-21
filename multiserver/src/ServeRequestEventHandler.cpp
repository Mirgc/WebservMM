#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ServeRequestEventHandler.hpp"
#include "Reactor.hpp"

const int BUFFER_SIZE = 30720;

ServeRequestEventHandler::ServeRequestEventHandler(Reactor& reactor, int fd)
    : IEventHandler(reactor, fd), reactor(reactor), fd(fd) {
    // Constructor implementation
}

void ServeRequestEventHandler::handleEvent() {
    char buffer[BUFFER_SIZE];
    size_t bytesRead = recv(fd, buffer, BUFFER_SIZE, 0);

    if (bytesRead == -1) {
        // Handle errors
    } else if (bytesRead == 0) {
        // Connection closed?
        reactor.unregisterEventHandler(fd);
    } else {
        // Process the received data, send responses back to the client here...
    }
}
