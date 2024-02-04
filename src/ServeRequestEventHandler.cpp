#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>
#include <signal.h>

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
        this->buffer = rhs.buffer;
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
            this->readOrCloseRequest();

            if (!this->isRequestHeaderFullyRead()) {
                // let this call go. Select will call again and will go straight to REQUEST_STATUS_READING_BODY
                break;
            }

            this->setRequestStatus(REQUEST_STATUS_READING_BODY);

        case REQUEST_STATUS_READING_BODY:
            if (!this->isRequestBodyFullyRead()) {
                // let this call go. Select will call again and will go straight to REQUEST_STATUS_READING_BODY
                break;
            }

            this->setRequestStatus(REQUEST_STATUS_PROCESSING);

        case REQUEST_STATUS_PROCESSING:
            this->processRequest();
            this->setRequestStatus(REQUEST_STATUS_PROCESSING_COMPLETE);

        case REQUEST_STATUS_PROCESSING_COMPLETE:
            this->setRequestStatus(REQUEST_STATUS_SENDING);

            // We break here, to let select call us again for writing.
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

            this->readOrCloseRequest();
            break;

        case REQUEST_STATUS_CLOSED_OK:
            break;

        case REQUEST_STATUS_CLOSED_ERROR:
        default:
            break;
    }
}

bool ServeRequestEventHandler::isRequestHeaderFullyRead() {
    return true;
}

bool ServeRequestEventHandler::isRequestBodyFullyRead() {
    return true;
}

bool ServeRequestEventHandler::isRequestFullyRead() {
    return this->isRequestHeaderFullyRead() && this->isRequestBodyFullyRead();
}

bool ServeRequestEventHandler::isRequestFullySent() {
    return true;
}

void ServeRequestEventHandler::setRequestStatus(t_http_request_status requestStatus)
{
#ifdef DEBUG_MODE
	bool isInvalidStatus = true;
    t_http_request_status currentStatus = this->requestStatus.getStatus();

    if (currentStatus == REQUEST_STATUS_WAITING)
	{
		isInvalidStatus = requestStatus != REQUEST_STATUS_WAITING &&
                          requestStatus != REQUEST_STATUS_READING_HEADERS &&
                          requestStatus != REQUEST_STATUS_CLOSED_OK;
	}
	else if (currentStatus == REQUEST_STATUS_READING_HEADERS)
	{
		isInvalidStatus = (requestStatus != REQUEST_STATUS_READING_BODY);
	}
	else if (currentStatus == REQUEST_STATUS_READING_BODY)
	{
		isInvalidStatus = (requestStatus != REQUEST_STATUS_PROCESSING);
	}
	else if (currentStatus == REQUEST_STATUS_PROCESSING)
	{
		isInvalidStatus = (requestStatus != REQUEST_STATUS_PROCESSING_COMPLETE);
	}
	else if (currentStatus == REQUEST_STATUS_PROCESSING_COMPLETE)
	{
		isInvalidStatus = (requestStatus != REQUEST_STATUS_SENDING);
	}
	else if (currentStatus == REQUEST_STATUS_SENDING)
	{
		isInvalidStatus = (requestStatus != REQUEST_STATUS_SENDING_COMPLETE);
	}
	else if (currentStatus == REQUEST_STATUS_SENDING_COMPLETE)
	{
		isInvalidStatus = (requestStatus != REQUEST_STATUS_CLOSED_OK) &&
                          (requestStatus != REQUEST_STATUS_PROCESSING) &&
                          (requestStatus != REQUEST_STATUS_CLOSED_ERROR);
	}

	if (isInvalidStatus)
	{
		std::cout << "ServeRequestEventHandler::setRequestStatus invalid set from " << (std::string)this->requestStatus << " to " << requestStatus << std::endl;
		throw std::runtime_error("erveRequestEventHandler::setRequestStatus error.");
	}
#endif

    this->requestStatus.setStatus(requestStatus);
    std::cout << "ServeRequestEventHandler::setRequestStatus on (fd = " << fd << ") to status " << (std::string)this->requestStatus << std::endl;
}

void ServeRequestEventHandler::processRequest() {
    HTTPRequestFactory httpRequestFactory;

    HTTPBody httpBody;
    httpBody.setBody(
        this->extractBodyFromHttpRequest(this->buffer)
    );

    HTTPHeader httpHeader;
    httpHeader.parseHTTPHeader(this->buffer);

    this->httpRequest = httpRequestFactory.createHTTPRequest(
        this->virtualHostServer.getServerConfig(),
        httpHeader,
        httpBody
    );

    if (!this->httpRequest) {
        this->buffer.clear();
        this->setRequestStatus(REQUEST_STATUS_CLOSED_ERROR);
        throw std::runtime_error("Error creating HTTPRequest from factory");
    }

    this->httpResponse = this->httpRequest->process();
    this->buffer.clear();
    this->freeHTTPRequest();
}

void ServeRequestEventHandler::sendResponse() {
    ssize_t bytesSent;
    std::string response = this->httpResponse.getResponse();

    bytesSent = send(fd, response.c_str(), response.size(), 0);

    // Process the received data, send responses back to the client here...
    std::cout << "ServeRequestEventHandler write data to client on (fd = " << fd << ") (bytesSent " << bytesSent << ")" << std::endl;
    std::cout << response << std::endl;

    if (bytesSent < (ssize_t) response.size()) {
        this->setRequestStatus(REQUEST_STATUS_CLOSED_ERROR);
        throw std::runtime_error("ServeRequestEventHandler::handleEvent: Error, not yet implemented");
    }
}

void ServeRequestEventHandler::readOrCloseRequest() {

    std::vector<char> tmpBuffer(BUFFER_SIZE);
    std::fill(tmpBuffer.begin(), tmpBuffer.end(), 0);

    std::cout << "ServeRequestEventHandler reading data from (fd = " << fd << ")" << std::endl;

    ssize_t bytesRead = recv(fd, &tmpBuffer[0], tmpBuffer.size(), 0);
    std::cout << "ServeRequestEventHandler read data from (fd = " << fd << ") (bytesRead = " << bytesRead << ")" << std::endl;

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
        // std::cout << buffer << std::endl;
        this->buffer.insert(this->buffer.end(), tmpBuffer.begin(), tmpBuffer.begin() + bytesRead);

        if (this->requestStatus.getStatus() == REQUEST_STATUS_SENDING_COMPLETE) {
            std::cout << "ServeRequestEventHandler connection has been reused as a keep-alive (fd = " << fd << ")." << std::endl;
            this->setRequestStatus(REQUEST_STATUS_PROCESSING);
            this->handleEvent();
        }
    }
}

std::vector<char> ServeRequestEventHandler::extractBodyFromHttpRequest(const std::vector<char> & httpRequest) const {
    const char *data = &httpRequest[0];
    const char *end = data + httpRequest.size();
    const char *delimiter = "\r\n\r\n";
    const char *pos = std::search(data, end, delimiter, delimiter + 4);

    if (pos != end) {
        return std::vector<char>(pos + 4, end);
    } else {
        return std::vector<char>();
    }
}
