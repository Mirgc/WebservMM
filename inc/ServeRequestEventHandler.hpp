#ifndef SERVER_REQUEST_EVENT_HANDLER_HPP
#define SERVER_REQUEST_EVENT_HANDLER_HPP

class Reactor;

#include "EventHandler.hpp"
#include "HTTPRequestStatus.hpp"
#include "HTTPResponse.hpp"

class HTTPRequest;

class ServeRequestEventHandler: public EventHandler {
public:
    ServeRequestEventHandler(Reactor& reactor, int fd, const VirtualHostServer & virtualHostServer);
    ServeRequestEventHandler(const ServeRequestEventHandler & src);
    ~ServeRequestEventHandler();

    ServeRequestEventHandler & operator=(ServeRequestEventHandler const & rhs);

    void handleEvent();

private:

    HTTPRequest *httpRequest;
    t_http_request_status requestStatus;
    ssize_t bytesRead;
    HTTPResponse httpResponse;

    void copyHTTPRequest(HTTPRequest * src);
    void freeHTTPRequest();
    bool isRequestHeaderFullyRead();
    bool isRequestBodyFullyRead();
    bool isRequestFullyRead();
    bool isRequestFullySent();
    void setRequestStatus(t_http_request_status requestStatus);
    void processRequest();
    void sendResponse();
    void confirmRequestClosedByClient();
    void readRequest();
};

#endif
