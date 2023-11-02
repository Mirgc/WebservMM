#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <sstream>

#include <fstream>
#include <iostream>

#include "ServeRequestEventHandler.hpp"
#include "Reactor.hpp"
#include "HTTPRequestCGI.hpp"

const int BUFFER_SIZE = 30720;

ServeRequestEventHandler::ServeRequestEventHandler(Reactor& reactor, int fd)
    : EventHandler(reactor, fd) {
    // Constructor implementation
}

void ServeRequestEventHandler::handleEvent() {
    char buffer[BUFFER_SIZE];

    memset(&buffer[0], 0, BUFFER_SIZE);
    size_t bytesRead = recv(fd, buffer, BUFFER_SIZE, 0);

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

        HTTPRequestCGI cgi;

        std::string cgiResponse = cgi.execCGI("cgi-bin/cgitest.py", "num1=11,num2=22");

        std::cout << "Parent returned content:" << std::endl;
        std::cout << cgiResponse << std::endl;

        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << cgiResponse.size() << "\n\n"
            << cgiResponse;

        std::string response = ss.str();

        int bytesSent;
        long totalBytesSent = 0;

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
