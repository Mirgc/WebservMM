#include <sys/socket.h>

#include "EventHandler.hpp"
#include "Reactor.hpp"

const int BUFFER_SIZE = 30720;

EventHandler::EventHandler(Reactor& reactor, int fd):
    reactor(reactor), fd(fd) {
}

void EventHandler::handleEvent() {
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
