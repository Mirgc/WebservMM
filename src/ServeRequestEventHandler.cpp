#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>

#include <fstream>
#include <iostream>

#include "ServeRequestEventHandler.hpp"
#include "Reactor.hpp"
#include "HTTPRequestFactory.hpp"
#include "HTTPResponse.hpp"

const int BUFFER_SIZE = 30720;

ServeRequestEventHandler::ServeRequestEventHandler(Reactor& reactor, int fd, const VirtualHostServer & virtualHostServer) 
    : EventHandler(reactor, fd, virtualHostServer), httpRequest(NULL) {
}

ServeRequestEventHandler::ServeRequestEventHandler(const ServeRequestEventHandler & src)
    : EventHandler(src.reactor, src.fd, src.virtualHostServer), httpRequest(NULL) {
    this->copyHTTPRequest(src.httpRequest);
}

ServeRequestEventHandler::~ServeRequestEventHandler() {
    this->freeHTTPRequest();
}

void ServeRequestEventHandler::copyHTTPRequest(HTTPRequest * httpRequest) {

    this->freeHTTPRequest();
    if (httpRequest) {
        this->httpRequest = httpRequest->clone();
    }
}

void ServeRequestEventHandler::freeHTTPRequest() {
    if (this->httpRequest) {
        delete this->httpRequest;
        this->httpRequest = NULL;
    }
}

ServeRequestEventHandler& ServeRequestEventHandler::operator=(const ServeRequestEventHandler &rhs) {
	if (this != &rhs) {
        this->fd = rhs.fd;
        this->virtualHostServer = rhs.virtualHostServer;
        this->copyHTTPRequest(rhs.httpRequest);
    }

	return (*this);
}

// TODO: Implement state machine: https://github.com/manu-garcia/WebServer/issues/53
void ServeRequestEventHandler::handleEvent() {
    char buffer[BUFFER_SIZE];

    memset(&buffer[0], 0, BUFFER_SIZE);
    ssize_t bytesRead = recv(fd, buffer, BUFFER_SIZE, 0);

    std::cout << std::endl << "ServeRequestEventHandler read data from (fd = " << fd << ") (bytesRead = " << bytesRead << ")" << std::endl;
    std::cout << buffer << std::endl;

    if (bytesRead == -1) {
        // Handle errors
        std::cout << "ServeRequestEventHandler error bytesRead -1" << std::endl;
    } else if (bytesRead == 0) {
        std::cout << "ServeRequestEventHandler connection closed by client" << std::endl;
        reactor.unregisterEventHandler(fd);
    } else {
        HTTPRequestFactory httpRequestFactory;
        this->httpRequest = httpRequestFactory.createHTTPRequest();
        if (!this->httpRequest) {
            throw std::runtime_error("Error creating HTTPRequest from factory");
        }

        HTTPResponse httpResponse = this->httpRequest->process();
        std::string response = httpResponse.getResponse();

        ssize_t bytesSent;

        bytesSent = send(fd, response.c_str(), response.size(), 0);

        // Process the received data, send responses back to the client here...
        std::cout << "ServeRequestEventHandler write data to client on (fd = " << fd << ") (bytesSent " << bytesSent << ")" << std::endl;
        if (bytesSent < (ssize_t) response.size()) {
            // TODO: This need proper implementation. Remember that we can only write once to a socket per select per socket
            // Still we need to control how much data has to be written, and how much data we have already wrote,
            // so that next time the socket is ready to write, we write only the remaining data.
            throw std::runtime_error("ServeRequestEventHandler::handleEvent: Error, not yet implemented");
        }
    }
}
