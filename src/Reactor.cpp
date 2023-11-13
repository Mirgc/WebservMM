#include <iostream>
#include <vector>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "Reactor.hpp"
#include "EventHandler.hpp"

Reactor *Reactor::instance = NULL;

Reactor::Reactor() {}

Reactor::Reactor(const Reactor & src) {
	*this = src;
}

Reactor::~Reactor() {}

Reactor& Reactor::operator=(const Reactor &rhs) {
    
	if (this != &rhs) {
        throw std::runtime_error("Reactor must be a Singleton and this case should never happen");
	}
	
	return (*this);
}

void Reactor::runEventLoop() {
    fd_set readSet;
    int maxFd = 0;

    // We will need to deal with ending this loop to free up and gracefully exit
    while (true) {
        FD_ZERO(&readSet);

        // Collects all registered socket fds
        for (std::map<int, EventHandler*>::iterator it = fdHandlerMap.begin(); it != fdHandlerMap.end(); ++it) {
            int fd = it->first;
            FD_SET(fd, &readSet);
            if (fd > maxFd) {
                maxFd = fd;
            }
        }

        // Checks whether any socket has data
        // TODO: Only one read or write per select per socket as requirement. We have to check writeSet also in select
        int numReady = select(maxFd + 1, &readSet, NULL, NULL, NULL);

        if (numReady == -1) {
            std::cerr << "select() error" << std::endl;
            return;
        }

        // Any with data?
        if (numReady > 0) {
            // Elements can get deleted from the map as we loop through it. Find a better solution
            for (std::map<int, EventHandler*>::iterator it = fdHandlerMap.begin(); it != fdHandlerMap.end(); ++it) {
                int fd = it->first;
                if (FD_ISSET(fd, &readSet)) {
                
                    std::cout << "Reactor socket (" << fd << ")with data..." << std::endl;

                    // Socket fd has data, disptach the event to the right handler
                    EventHandler* handler = it->second;
                    handler->handleEvent();
                }
            }
        }
    }
}

void Reactor::stopEventLoop() {
    // Implement stopping the event loop (while true for now)
}

void Reactor::registerEventHandler(int fd, EventHandler* handler) {
    fdHandlerMap[fd] = handler;
}

void Reactor::unregisterEventHandler(int fd) {
    std::map<int, EventHandler*>::iterator it = fdHandlerMap.find(fd);
    if (it != fdHandlerMap.end()) {
        // Removes the fd, EventHandler from the map
        fdHandlerMap[fd] = NULL;
        // Deletes the instance of the class EventHandler
        delete it->second;
        // Closes the fd
        close(fd);
    }
}