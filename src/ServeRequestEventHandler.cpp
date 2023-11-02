#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <sstream>

#include <fstream>
#include <iostream>

#include "ServeRequestEventHandler.hpp"
#include "Reactor.hpp"

const int BUFFER_SIZE = 30720;

ServeRequestEventHandler::ServeRequestEventHandler(Reactor& reactor, int fd)
    : EventHandler(reactor, fd) {
    // Constructor implementation
}

ServeRequestEventHandler::~ServeRequestEventHandler() {}

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

        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
            << htmlFile;

        std::string response = ss.str();

        size_t bytesSent;
        size_t totalBytesSent = 0;

        // TODO: This is only an example. Remember that we can only write once to a socket per select per socket
        // Still we need to control how much data has to be written, and how much data we have already wrote,
        // so that next time the socket is ready to write, we write only the remaining data.
        while (totalBytesSent < response.size())
        {
            bytesSent = send(fd, response.c_str(), response.size(), 0);
            if (bytesSent < 0)
            {
                break;
            }
            totalBytesSent += bytesSent;
        }
    }
}
