#ifndef VIRTUAL_HOST_SERVER_HPP
#define VIRTUAL_HOST_SERVER_HPP

#include "Reactor.hpp"

class VirtualHostServer {
public:
    VirtualHostServer(Reactor& reactor, int port);
    VirtualHostServer(const VirtualHostServer & src);
    ~VirtualHostServer();

    VirtualHostServer & operator=(VirtualHostServer const & rhs);

    void listen();

private:
    Reactor& reactor;
    int port;
    int listenSocket;
};

#endif
