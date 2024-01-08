#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>

#include <fstream>
#include <iostream>

#include "VirtualHostServer.hpp"
#include "AcceptConnectionEventHandler.hpp"

VirtualHostServer::VirtualHostServer(Reactor& reactor, const ServerConfig & config):
    reactor(reactor), config(config) {
}

VirtualHostServer::VirtualHostServer(const VirtualHostServer & src) : reactor(src.reactor) {
	*this = src;
}

VirtualHostServer::~VirtualHostServer() {}

VirtualHostServer& VirtualHostServer::operator=(const VirtualHostServer &rhs) {
	if (this != &rhs) {
        this->config = rhs.config;
        this->listenSocket = rhs.listenSocket;
	}
	return (*this);
}

void VirtualHostServer::start()
{
    // Create a socket for listening
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0) {
        throw std::runtime_error("Failed to create a listening socket");
    }

    // Set up the address for IP4 on 0.0.0.0:port
    Address.sin_family = AF_INET;
    Address.sin_addr.s_addr = INADDR_ANY;
    Address.sin_port = htons(config.getPortAt(0));

    int reuse = 1;
    if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        close(listenSocket);
        throw std::runtime_error("Failed to set socket option");
    }

    // Bind the socket to the address
    if (bind(listenSocket, (struct sockaddr*)&Address, sizeof(Address)) < 0) {
        close(listenSocket);
        throw std::runtime_error("Failed to bind the listening socket");
    }

    listen();
}

void VirtualHostServer::listen() {
    // Start listening for incoming connections
    if (::listen(listenSocket, SOMAXCONN) == -1) {
        close(listenSocket);
        throw std::runtime_error("Failed to start listening on the socket");
    }

    std::cout << "Registering event (fd = " << listenSocket << ") AcceptConnectionEventHandler" << std::endl;

    // This handler will accept new connections to this Server/VirtualHost
    EventHandler *acceptNewConnectionHandler = new AcceptConnectionEventHandler(reactor, listenSocket, *this);
    reactor.registerEventHandler(listenSocket, acceptNewConnectionHandler);
}

void VirtualHostServer::stop()
{

    std::cout << "VirtualHostServer::stop() closing the server socket!!" << std::endl;

    if (listenSocket) {
        close(listenSocket);
        listenSocket = 0;
    }
}

struct sockaddr_in VirtualHostServer::getAddress(void) {
    return (Address);
}

unsigned int VirtualHostServer::getPort() const {
    return this->config.getPortAt(0);
}
