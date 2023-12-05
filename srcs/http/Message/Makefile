# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/13 18:31:50 by jyao              #+#    #+#              #
#    Updated: 2023/12/06 01:21:28 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile Message -*-

NAME			:=	libmessage.a

HEADERS			:=	AMessage.hpp	Header.hpp	Http_namespace.hpp	\
				Request.hpp		Response.hpp
HTTP_DIR		:=	../
CGI_DIR			:=	../CGIhandler/
CONFIG_DIR		:=	../../ServerConfig/
AUTOINDEX_DIR	:=	../Autoindex/
INCLUDES		:=	-I.	-I$(CONFIG_DIR) -I$(CGI_DIR) -I$(AUTOINDEX_DIR) -I$(HTTP_DIR)
CXX				=	c++
FILES			:=	AMessage	Header	Request	Response	MimeTypes
SRCS			=	$(addsuffix .cpp, $(FILES))

OBJS			=	${SRCS:.cpp=.o}
CXXFLAGS		=	-Wall -Wextra -Werror -g3 -std=c++98

all: $(NAME)

$(NAME): $(HEADERS) $(OBJS)
	ar -rvs $@ $(OBJS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES)  -o $@ -c $<

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re