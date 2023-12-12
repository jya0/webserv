# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/03 13:29:44 by jyao              #+#    #+#              #
#    Updated: 2023/12/13 01:27:44 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile server

CXX				:=	c++
# CXXFLAGS		:= -Wall -Wextra -Werror -fsanitize=address -g3 -std=c++98 -fPIC
CXXFLAGS		:=	-Wall -Wextra -Werror -g3 -std=c++98 -fPIC


UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
LIB_SUFFIX	:=	.so
LIB_COMPILE	:=	$(CXX) $(CXXFLAGS) -shared -o
endif
ifeq ($(UNAME), Darwin)
LIB_SUFFIX	:=	.a
LIB_COMPILE	:=	ar -rvs
endif

NAME			=	$(addsuffix $(LIB_SUFFIX), libserver)

HTTP_DIR		:=	../http/
HTTP_LIB		=	$(HTTP_DIR)libhttp.a
MESSAGE_DIR		=	$(HTTP_DIR)Message/
CGI_DIR			:=	$(HTTP_DIR)CGIhandler/
AUTOINDEX_DIR	:=	$(HTTP_DIR)Autoindex/
CONFIG_DIR		:=	../ServerConfig/
CONFIG_LIB		=	$(CONFIG_DIR)libconfparser.a

HEADERS			:=	PollManager.hpp	ServerMonitor.hpp	ServerSocket.hpp	\
					WebServer.hpp

LIBS			=	$(HTTP_LIB) $(CONFIG_LIB)

INCLUDES		:=	-I.	-I$(HTTP_DIR) -I$(CGI_DIR) -I$(AUTOINDEX_DIR) -I$(CONFIG_DIR) -I$(MESSAGE_DIR)

RM				=	rm

FILES			:=	PollManager		ServerSocket	\
					WebServer		ServerMonitor

SRCS			=	$(addsuffix .cpp, $(FILES))

OBJS			=	$(addsuffix .o, $(FILES))


all:	$(NAME)

$(NAME): $(HEADERS) $(OBJS)
	$(LIB_COMPILE) $@ $(OBJS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	$(RM) -rf $(OBJS)

fclean:	clean
	$(RM) -rf $(NAME)

re:		fclean all

.PHONY: all clean fclean re