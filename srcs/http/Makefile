# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/13 18:31:50 by jyao              #+#    #+#              #
#    Updated: 2023/12/04 03:44:00 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile -*-

NAME			:=	libhttp.a

# LIB			=	./lib/
HEADERS			:=	AMessage.hpp	Header.hpp	Http_namespace.hpp	\
				Request.hpp		Response.hpp
CGI_DIR			:=	./CGIhandler/
CONFIG_DIR		:=	../ServerConfig/
AUTOINDEX_DIR	:=	./Autoindex/
INCLUDES		:=	-I. -I../inc/	-I$(CONFIG_DIR) -I$(CGI_DIR) -I$(AUTOINDEX_DIR)
CXX				=	c++
FILES			:=	AMessage	Header	Request	Response	MimeTypes
SRCS			=	$(addsuffix .cpp, $(FILES))

# srcs/cgi/
# srcs/http/
# srcs/parser/
# srcs/socket/

OBJS		=	${SRCS:.cpp=.o}
CXXFLAGS		=	-Wall -Wextra -Werror -g3 -std=c++98

all: $(NAME)

$(NAME): $(HEADERS) $(OBJS)
		ar -rvs $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

# $(CXX) $(CXXFLAGS) -I ../inc/webserv.h  $(OBJS) -o $(NAME)

# $(LIB):
# 		#make -C ./lib/

clean:
		rm -rf $(OBJS)
fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re