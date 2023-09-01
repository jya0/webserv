
#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP
# include <string>
# include <vector>

enum RequestType {
    GET,
    POST,
    HEAD,
    PUT,
    DELETE
};

class HTTPRequest {
  public:
    RequestType type;
    std::string uri;
    std::string httpVersion;
    std::vector<std::string> headers;
    std::string body;
    HTTPRequest();
    HTTPRequest(std::string rawRequest);
    ~HTTPRequest();
};

#endif