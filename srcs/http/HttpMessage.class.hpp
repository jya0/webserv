#ifndef HTTP_MESSAGE_CLASS_HPP
# define HTTP_MESSAGE_CLASS_HPP

# include	<iostream>
# include	<string>
# include	<list>
# include	<utility>

namespace	http
{
	class	HttpMessage
	{
		private:
		protected:
			int	_httpVersion;
			std::list<std::pair<std::string, std::string> >	_headers;
			std::string	_messageBody;
		public:
			HttpMessage(void);
			HttpMessage(HttpMessage &httpMessageREF);
			~HttpMessage(void);
			HttpMessage	&operator=(HttpMessage &httpMessageREF);

			std::string	loadHttpRequest(int fd);
			HttpMessage *parseHttpMessage(std::string httpRequestRaw);
	};
}

#endif