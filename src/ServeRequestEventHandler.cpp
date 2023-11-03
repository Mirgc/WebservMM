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

ServeRequestEventHandler::ServeRequestEventHandler(Reactor& reactor, int fd)
    : EventHandler(reactor, fd) {
    this->httpRequest = NULL;
}

ServeRequestEventHandler::ServeRequestEventHandler(const ServeRequestEventHandler & src)
    : EventHandler(src.reactor, src.fd) {
    this->copyHTTPRequest(src.httpRequest);
}

ServeRequestEventHandler::~ServeRequestEventHandler() {
    this->freeHTTPRequest();
}

void ServeRequestEventHandler::copyHTTPRequest(HTTPRequest * src) {

    this->freeHTTPRequest();
    if (src) {
        // TODO: HTTPRequest is an abstract class. Find out how to copy it.
        throw std::runtime_error("Error, copy constructor for ServeRequestEventHandler not yet supported.");
        // this->httpRequest = new HTTPRequest(*(src));
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
        this->reactor = rhs.reactor;
        this->fd = rhs.fd;
        this->copyHTTPRequest(rhs.httpRequest);
    }

	return (*this);
}

// TODO: Implement state machine: https://github.com/manu-garcia/WebServer/issues/53
void ServeRequestEventHandler::handleEvent() {
    char buffer[BUFFER_SIZE];

    memset(&buffer[0], 0, BUFFER_SIZE);
    ssize_t bytesRead = recv(fd, buffer, BUFFER_SIZE, 0);

    std::cout << std::endl << "ServeRequestEventHandler recv (fd = " << fd << ")(bytesRead = " << bytesRead << ")" << std::endl;
    std::cout << buffer << std::endl;

    if (bytesRead == -1) {
        // Handle errors
        std::cout << "ServeRequestEventHandler error bytesRead -1" << std::endl;
    } else if (bytesRead == 0) {
        // Connection closed?
        std::cout << "ServeRequestEventHandler connection closed? bytesRead 0" << std::endl;
        reactor.unregisterEventHandler(fd);
    } else {
        // Process the received data, send responses back to the client here...
        std::cout << "ServeRequestEventHandler bytesRead n, Sending response back" << std::endl;

        HTTPRequestFactory httpRequestFactory;
        this->httpRequest = httpRequestFactory.createHTTPRequest();
        if (!this->httpRequest) {
            throw std::runtime_error("Error creating HTTPRequest from factory");
        }

        HTTPResponse httpResponse = this->httpRequest->process();
        std::string response = httpResponse.getResponse();

        ssize_t bytesSent;
        size_t totalBytesSent = 0;

        // TODO: This is only an example. Remember that we can only write once to a socket per select per socket
        // Still we need to control how much data has to be written, and how much data we have already wrote,
        // so that next time the socket is ready to write, we write only the remaining data.
        while (totalBytesSent < response.size())
        {
            bytesSent = send(fd, response.c_str(), response.size(), 0);
            if (bytesSent <= 0)
            {
                break;
            }
            totalBytesSent += bytesSent;
        }
    }
}
