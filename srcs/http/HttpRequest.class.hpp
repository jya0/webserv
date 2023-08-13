#ifndef HTTP_REQUEST_CLASS_HPP
# define HTTP_REQUEST_CLASS_HPP

# include	<iostream>
# include	<string>

namespace	http
{
	class	HttpResponse: private HttpMessage
	{
		private:
			int	_httpStatusCode;
		protected:
		public:
			HttpResponse(void);
			HttpResponse(HttpResponse &httpResponseREF);
			~HttpResponse(void);
			HttpResponse	&operator=(HttpResponse &httpResponseREF);
	};
}


#endif