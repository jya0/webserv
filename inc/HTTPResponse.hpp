#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP
# include <string>
# include <vector>

class HTTPResponse {
  public:
    std::string httpVersion;
    unsigned short statusCode;
    std::string reasonPhrase;
    std::vector<std::string> headers;
    std::string body;
    HTTPResponse();
    HTTPResponse(std::string rawResponse);
    ~HTTPResponse();
};

#endif