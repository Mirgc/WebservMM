#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>

#include "VirtualHostServer.hpp"
#include "AcceptConnectionEventHandler.hpp"

VirtualHostServer::VirtualHostServer(Reactor& reactor, int port) : reactor(reactor) {
    // Create a socket for listening
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0) {
        throw std::runtime_error("Failed to create a listening socket");
    }

    // Set up the address for IP4 on 0.0.0.0:port
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    // Bind the socket to the address
    if (bind(listenSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        close(listenSocket);
        throw std::runtime_error("Failed to bind the listening socket");
    }
}

void VirtualHostServer::listen() {
    // Start listening for incoming connections
    if (::listen(listenSocket, SOMAXCONN) == -1) {
        close(listenSocket);
        throw std::runtime_error("Failed to start listening on the socket");
    }

    // This handler will accept new connections to this Server/VirtualHost
    // Event Handler should be an abstract, because we will need multiple different
    // Event Handlers for: AcceptingNewConnection, HandlingHTTPRequestStaticFile,
    // HandlingHTTPRequestCGI, HandlingHTTPRequestRedirection, HandlingHTTPRequestListDir, etc...
    IEventHandler *acceptNewConnectionHandler = new AcceptConnectionEventHandler(reactor, listenSocket);
    reactor.registerEventHandler(listenSocket, acceptNewConnectionHandler);
}
