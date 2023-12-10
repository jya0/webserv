# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/13 18:31:50 by jyao              #+#    #+#              #
#    Updated: 2023/12/10 18:26:00 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile Message -*-

CXX				:=	c++ 
CXXFLAGS		:=	-Wall -Wextra -Werror -g3 -fsanitize=address -std=c++98 -fPIC

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
LIB_SUFFIX	:=	.so
LIB_COMPILE	:=	$(CXX) $(CXXFLAGS) -shared -o
endif
ifeq ($(UNAME), Darwin)
LIB_SUFFIX	:=	.a
LIB_COMPILE	:=	ar -rvs
endif

NAME		=	$(addsuffix $(LIB_SUFFIX), libmessage)

HEADERS			:=	AMessage.hpp	Header.hpp	Http_namespace.hpp	\
				Request.hpp		Response.hpp
HTTP_DIR		:=	../
CGI_DIR			:=	../CGIhandler/
CONFIG_DIR		:=	../../ServerConfig/
AUTOINDEX_DIR	:=	../Autoindex/
INCLUDES		:=	-I.	-I$(CONFIG_DIR) -I$(CGI_DIR) -I$(AUTOINDEX_DIR) -I$(HTTP_DIR)
FILES			:=	AMessage	Header	Request	Response	MimeTypes
SRCS			=	$(addsuffix .cpp, $(FILES))

OBJS			=	${SRCS:.cpp=.o}

all: $(NAME)

$(NAME): $(HEADERS) $(OBJS)
	$(LIB_COMPILE) $@ $(OBJS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re