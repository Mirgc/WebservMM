#ifndef HTTP_REQUEST_STATUS
#define HTTP_REQUEST_STATUS

#include <string>

typedef enum e_http_request_status
{
    // Serving the request process has not yet started.
    REQUEST_STATUS_WAITING = 0,

    // We have received none or part of the headers. Not all.
    REQUEST_STATUS_READING_HEADERS = 1,
    // We have received none or part of the body. Not all.
    REQUEST_STATUS_READING_BODY = 2,

    // We are currently processing the request; calling cgi, reading file, etc...
    REQUEST_STATUS_PROCESSING = 3,
    // Incoming request has been fully processed.
    REQUEST_STATUS_PROCESSING_COMPLETE = 4,

    // We have sent none or part of the request to the client. Not all.
    REQUEST_STATUS_SENDING = 5,
    // Incoming request has been fully sent to the client.
    REQUEST_STATUS_SENDING_COMPLETE = 6,

    REQUEST_STATUS_CLOSED_OK = 7,
    REQUEST_STATUS_CLOSED_ERROR = 8
} t_http_request_status;

#endif
