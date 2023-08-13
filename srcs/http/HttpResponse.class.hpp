#ifndef HTTP_RESPONSE_CLASS_HPP
# define HTTP_RESPONSE_CLASS_HPP

# include	<iostream>
# include	<string>
# include	"HttpMessage.class.hpp"
# include	"HttpResponse.class.hpp"

namespace	http
{
	class	HttpRequest: private HttpMessage
	{
		private:
			std::string	_httpMethod;
			std::string	_uri;
		protected:
		public:
			HttpRequest(void);
			HttpRequest(HttpRequest &httpRequestREF);
			~HttpRequest(void);
			HttpRequest	&operator=(HttpRequest &httpRequestREF);

			HttpResponse	*httpGet();
			HttpResponse	*httpPost();
			HttpResponse	*httpPut();
			HttpResponse	*httpDelete();
	};
}

#endif