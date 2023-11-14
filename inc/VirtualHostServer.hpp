#ifndef VIRTUAL_HOST_SERVER_HPP
#define VIRTUAL_HOST_SERVER_HPP

#include "Reactor.hpp"
#include "ServerConfig.hpp"

class VirtualHostServer {
public:
    VirtualHostServer(Reactor& reactor, const ServerConfig & config);
    VirtualHostServer(const VirtualHostServer & src);
    ~VirtualHostServer();

    VirtualHostServer & operator=(VirtualHostServer const & rhs);

    void listen();

private:
    Reactor& reactor;
    ServerConfig config;
    int listenSocket;
};

#endif
