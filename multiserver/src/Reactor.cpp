#include <iostream>
#include <vector>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "Reactor.hpp"
#include "EventHandler.hpp"

Reactor::Reactor() {}

void Reactor::runEventLoop() {
    fd_set readSet;
    int maxFd = 0;

    // We will need to deal with ending this loop to free up and gracefully exit
    while (true) {
        FD_ZERO(&readSet);

        // Collects all registered socket fds
        for (std::set<std::pair<int, IEventHandler*> >::iterator it = fdHandlerPairs.begin(); it != fdHandlerPairs.end(); ++it) {
            int fd = it->first;
            FD_SET(fd, &readSet);
            if (fd > maxFd) {
                maxFd = fd;
            }
        }

        // Checks whether any socket has data
        int numReady = select(maxFd + 1, &readSet, NULL, NULL, NULL);

        if (numReady == -1) {
            std::cerr << "select() error" << std::endl;
            return;
        }

        // Any with data?
        if (numReady > 0) {
            for (std::set<std::pair<int, IEventHandler*> >::iterator it = fdHandlerPairs.begin(); it != fdHandlerPairs.end(); ++it) {
                int fd = it->first;
                if (FD_ISSET(fd, &readSet)) {
                    // Socket fd has data, disptach the event to the right handler
                    IEventHandler* handler = it->second;
                    handler->handleEvent();
                }
            }
        }
    }
}

void Reactor::stopEventLoop() {
    // Implement stopping the event loop (while true for now)
}

void Reactor::registerEventHandler(int fd, IEventHandler* handler) {
    fdHandlerPairs.insert(std::make_pair(fd, handler));
}

void Reactor::unregisterEventHandler(int fd) {
    std::set<std::pair<int, IEventHandler*> >::iterator it = fdHandlerPairs.begin();
    while (it != fdHandlerPairs.end()) {
        if (it->first == fd) {
            // Delete handler instance here?
            it = fdHandlerPairs.erase(it);
        } else {
            ++it;
        }
    }
}
