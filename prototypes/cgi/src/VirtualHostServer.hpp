#ifndef VIRTUAL_HOST_SERVER_HPP
#define VIRTUAL_HOST_SERVER_HPP

#include "Reactor.hpp"

class VirtualHostServer {
public:
    VirtualHostServer(Reactor& reactor, int port);
    void listen();

private:
    Reactor& reactor;
    int listenSocket;
};

#endif
