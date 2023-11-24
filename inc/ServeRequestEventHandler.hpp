#ifndef SERVER_REQUEST_EVENT_HANDLER_HPP
#define SERVER_REQUEST_EVENT_HANDLER_HPP

class Reactor;

#include "EventHandler.hpp"
#include "HTTPRequest.hpp"

class ServeRequestEventHandler: public EventHandler {
public:
    ServeRequestEventHandler(Reactor& reactor, int fd, const VirtualHostServer & virtualHostServer);
    ServeRequestEventHandler(const ServeRequestEventHandler & src);
    ~ServeRequestEventHandler();

    ServeRequestEventHandler & operator=(ServeRequestEventHandler const & rhs);

    void handleEvent();

private:

    HTTPRequest *httpRequest;

    void copyHTTPRequest(HTTPRequest * src);
    void freeHTTPRequest();
};

#endif
