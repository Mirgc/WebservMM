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
#include "HTTPHeader.hpp"
#include "HTTPBody.hpp"

// To be done:

// - ServeRequestEventHandler: Read data in chunks, let select call us again. Upload a big file with POST to test
// - ServeRequestEventHandler: Find a way to determine isRequestHeaderFullyRead and implement
// - ServeRequestEventHandler: Find a way to determine isRequestBodyFullyRead and implement

// - ServeRequestEventHandler: Write data in chunks, let select call us again. Download a big file with GET to test
// - ServeRequestEventHandler: Find a way to determine isRequestFullySent and implement

// - ServeRequestEventHandler: Check that a status can move from a valid previous one in setRequestStatus

ServeRequestEventHandler::ServeRequestEventHandler(Reactor& reactor, int fd, const VirtualHostServer & virtualHostServer) 
    : EventHandler(reactor, fd, virtualHostServer) {
    this->httpRequest = NULL;
    this->setRequestStatus(REQUEST_STATUS_WAITING);
    this->bytesRead = 0;
}

ServeRequestEventHandler::ServeRequestEventHandler(const ServeRequestEventHandler & src)
    : EventHandler(src.reactor, src.fd, src.virtualHostServer) {
    this->requestStatus = src.requestStatus;
    this->bytesRead = src.bytesRead;
    this->httpResponse = src.httpResponse;
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
        this->requestStatus = rhs.requestStatus;
        this->bytesRead = rhs.bytesRead;
        this->httpResponse = rhs.httpResponse;
        // TODO: This will be moved to a property class to handle the buffer properly
        memcpy(&this->buffer, &rhs.buffer, BUFFER_SIZE);
        this->copyHTTPRequest(rhs.httpRequest);
    }

	return (*this);
}

void ServeRequestEventHandler::handleEvent(const t_event_handler_type eventType) {

    std::cout << "ServeRequestEventHandler::handleEvent() (fd = " << fd << ") (request status: " << (std::string)this->requestStatus << ")" << std::endl;

    switch(this->requestStatus.getStatus()) {
        case REQUEST_STATUS_WAITING:

            if (eventType != EVENT_HANDLER_TYPE_READ) {
                std::cout << "ServeRequestEventHandler::handleEvent() REQUEST_STATUS_WAITING ignoring write event when no data has been read yet." << std::endl;
                this->setRequestStatus(REQUEST_STATUS_CLOSED_OK);
                reactor.unregisterEventHandler(fd);
                break;
            }

            this->setRequestStatus(REQUEST_STATUS_READING_HEADERS);
        case REQUEST_STATUS_READING_HEADERS:
            // TODO: How do we know here we have or not read the whole headers request?
            // If we have, we can parse the headers here.
            // Can headers be sent in multiple chunks?
            // Check the RFC
            // Headers are separated by a blank line from the body
            this->readOrCloseRequest();

            if (!this->isRequestHeaderFullyRead()) {
                // let this call go. Select will call again and will go straight to REQUEST_STATUS_READING_BODY
                break;
            }

            this->setRequestStatus(REQUEST_STATUS_READING_BODY);

        case REQUEST_STATUS_READING_BODY:
            // TODO: How do we know here we have or not read the whole incoming body
            // Test a very large file upload, as the file comes in the body, propably would need to be read 
            // in multiple times (select would sign read ready for this socket multiple times?)
            // The body is optional. I believe GET requests do not have a body.
            // Check the RFC

            if (!this->isRequestBodyFullyRead()) {
                // let this call go. Select will call again and will go straight to REQUEST_STATUS_READING_BODY
                break;
            }

            this->setRequestStatus(REQUEST_STATUS_PROCESSING);

        case REQUEST_STATUS_PROCESSING:
            this->processRequest();
            this->setRequestStatus(REQUEST_STATUS_PROCESSING_COMPLETE);

        case REQUEST_STATUS_PROCESSING_COMPLETE:
            // TODO: We should not write right now, only when select() says the socket is
            // ready to be written to.
            this->setRequestStatus(REQUEST_STATUS_SENDING);

            // TODO: Commented out until we implement writing sockets!
            break;

        case REQUEST_STATUS_SENDING:
            this->sendResponse();
            if (!this->isRequestFullySent()) {
                // let this call go. Select will call again and will go straight to REQUEST_STATUS_SENDING
                break;
            }

            this->setRequestStatus(REQUEST_STATUS_SENDING_COMPLETE);
            break;

        case REQUEST_STATUS_SENDING_COMPLETE:

            if (eventType != EVENT_HANDLER_TYPE_READ) {
                std::cout << "ServeRequestEventHandler::handleEvent() REQUEST_STATUS_SENDING_COMPLETE ignoring write event when no data has been read yet." << std::endl;
                this->setRequestStatus(REQUEST_STATUS_CLOSED_OK);
                reactor.unregisterEventHandler(fd);
                break;
            }

            // TODO: We have to control here connection keep alive. That would do for now
            // until we have the headers
            this->readOrCloseRequest();
            break;

        case REQUEST_STATUS_CLOSED_OK:
            break;

        // TODO: Error case, to decide what to do
        case REQUEST_STATUS_CLOSED_ERROR:
        default:
            break;
    }
}

bool ServeRequestEventHandler::isRequestHeaderFullyRead() {
    // TODO: Implement this full headers should have been read
    return true;
}

bool ServeRequestEventHandler::isRequestBodyFullyRead() {
    // TODO: Implement this full body should have been read
    // If GET there is no body
    return true;
}

bool ServeRequestEventHandler::isRequestFullyRead() {
    return this->isRequestHeaderFullyRead() && this->isRequestBodyFullyRead();
}

bool ServeRequestEventHandler::isRequestFullySent() {
    // TODO: Implement this to controll how many bytes have we sent to the client
    // wrtting to a socket returns how many bytes have been sent, if the size of the 
    // request is bigger, let go, and select() should signal this socket to be written
    // to again
    return true;
}

void ServeRequestEventHandler::setRequestStatus(t_http_request_status requestStatus)
{
    // TODO: We can check here that the current status is a valid status to move from
    // to the new status.
    // for instance we should not jump from waiting, to sending, because we havent read the request!
    this->requestStatus.setStatus(requestStatus);
    std::cout << "ServeRequestEventHandler::setRequestStatus on (fd = " << fd << ") to status " << (std::string)this->requestStatus << std::endl;
}

void ServeRequestEventHandler::processRequest() {
    HTTPRequestFactory httpRequestFactory;

    // TODO: Body remains empty for now until we work on moving the buffer to it own class
    HTTPBody httpBody;
    HTTPHeader httpHeader;

    httpHeader.parseHTTPHeader(this->buffer);

    this->httpRequest = httpRequestFactory.createHTTPRequest(
        this->virtualHostServer.getServerConfig(),
        httpHeader,
        httpBody
    );

    if (!this->httpRequest) {
        this->setRequestStatus(REQUEST_STATUS_CLOSED_ERROR);
        throw std::runtime_error("Error creating HTTPRequest from factory");
    }

    this->httpResponse = this->httpRequest->process();
    this->freeHTTPRequest();
}

void ServeRequestEventHandler::sendResponse() {
    ssize_t bytesSent;
    std::string response = this->httpResponse.getResponse();

    // TODO: To be removed. For now we tell the broser that there is no favico file
    if (this->bIsFaviconRequest) {
        std::string bodyContent = "<!DOCTYPE html><html><body><h1>404 Not Found.</h1></body></html>";
        std::stringstream ss;

        ss << "HTTP/1.1 404\r\n"
            << "Content-Type: text/html\r\n"
            << "Content-Length: " << bodyContent.size()
            << "\r\n\r\n"
            << bodyContent;

        response = ss.str();
    }
    else
    {
        response = this->httpResponse.getResponse();
    }


    bytesSent = send(fd, response.c_str(), response.size(), 0);

    // Process the received data, send responses back to the client here...
    std::cout << "ServeRequestEventHandler write data to client on (fd = " << fd << ") (bytesSent " << bytesSent << ")" << std::endl;
    if (bytesSent < (ssize_t) response.size()) {
        // TODO: This need proper implementation. Remember that we can only write once to a socket per select per socket
        // Still we need to control how much data has to be written, and how much data we have already wrote,
        // so that next time the socket is ready to write, we write only the remaining data.
        this->setRequestStatus(REQUEST_STATUS_CLOSED_ERROR);
        throw std::runtime_error("ServeRequestEventHandler::handleEvent: Error, not yet implemented");
    }
}

void ServeRequestEventHandler::readOrCloseRequest() {

    // TODO: This needs to be improved with the issue about reading in chunks.
    memset(&this->buffer, 0, BUFFER_SIZE);

    std::cout << "ServeRequestEventHandler reading data from (fd = " << fd << ")" << std::endl;

    bytesRead = recv(fd, buffer, BUFFER_SIZE, 0);
    std::cout << "ServeRequestEventHandler read data from (fd = " << fd << ") (bytesRead = " << bytesRead << ")" << std::endl;

    // TODO: Remove hardcoded check to differentiate from the index.hml and the favico
    // GET /favicon.ico HTTP/1.1
    // We always return for now a hardocded html even when chrome requests the favico
    // We are confusing Chrome 
    if (strstr(buffer, "GET /favicon.ico") != NULL) {
        bIsFaviconRequest = true;
    } else {
        bIsFaviconRequest = false;
    }

    if (bytesRead == -1) {
        this->setRequestStatus(REQUEST_STATUS_CLOSED_ERROR);
        std::runtime_error("ServeRequestEventHandler error bytesRead -1");
    } else if (bytesRead == 0) {
        if (this->requestStatus.getStatus() == REQUEST_STATUS_SENDING_COMPLETE) {
            // Normal closure of a connection
            this->setRequestStatus(REQUEST_STATUS_CLOSED_OK);
            std::cout << "ServeRequestEventHandler connection closed by client" << std::endl;
        } else {
            this->setRequestStatus(REQUEST_STATUS_CLOSED_ERROR);
            std::cout << "ServeRequestEventHandler unexpected connection closed by client" << std::endl;
        }
        reactor.unregisterEventHandler(fd);
    } else {
        std::cout << buffer << std::endl;

        if (this->requestStatus.getStatus() == REQUEST_STATUS_SENDING_COMPLETE) {
            // TODO: The connection is probably a keep alive connection. It is now being reused.
            // We have to deal properly with keep alive connection.
            // This is a temporal workaround
            // TODO: We are not controlling here the reading in chunks!!! This is just a workaround
            //until we have the headers and implement keep alive properly
            std::cout << "ServeRequestEventHandler connection has been reused as a keep-alive (fd = " << fd << ")." << std::endl;
            this->setRequestStatus(REQUEST_STATUS_PROCESSING);
            this->handleEvent();
        }
    }
}
