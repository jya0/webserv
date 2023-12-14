# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 14:58:03 by jyao              #+#    #+#              #
#    Updated: 2023/12/14 09:02:11 by rriyas           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile Autoindex -*-

NAME			=	webserv

CXX				=	c++
# CXXFLAGS		=	-Wall -Wextra -Werror -g3 -std=c++98
CXXFLAGS		=	-Wall -Wextra -Werror -g3 -fsanitize=address -std=c++98
# CXXFLAGS		:=	-Wall -Wextra -Werror -std=c++98
# CXXFLAGS		:=	-Wall -Wextra -Werror -std=c++98 -O2


UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
LIB_SUFFIX	:=	.so
LIB_COMPILE	:=	$(CXX) $(CXXFLAGS) -shared -o
endif
ifeq ($(UNAME), Darwin)
LIB_SUFFIX	:=	.a
LIB_COMPILE	:=	ar -rvs
endif

NAME		=	$(addsuffix $(LIB_SUFFIX), libautoindex)
# NAME		:=	autoIndexTest

MESSAGE_DIR		:=	../Message/
CONFIG_DIR		:=	../../ServerConfig/
HTTP_DIR		:=	../

HEADERS			:=	PollManager.hpp	ServerMonitor.hpp	ServerSocket.hpp	\
					WebServer.hpp

INCLUDES		:=	-I.	-I$(MESSAGE_DIR) -I$(CONFIG_DIR) -I$(HTTP_DIR)

HEADERS		:=	Autoindex_namespace.hpp

FILES		:=	Autoindex


RM			:=	rm

SRCS		=	$(addsuffix .cpp, $(FILES))

OBJS		=	$(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(HEADERS) $(OBJS)
	$(LIB_COMPILE) $@ $(OBJS)
#$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	$(RM) -rf $(OBJS)

fclean:	clean
	$(RM) -rf $(NAME)

re:		fclean all

.PHONY: all clean fclean re
