# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 14:58:03 by jyao              #+#    #+#              #
#    Updated: 2023/12/06 13:22:26 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile Autoindex -*-

NAME		:=	libautoindex.so
# NAME		:=	autoIndexTest

MESSAGE_DIR		:=	../Message/
CONFIG_DIR		:=	../../ServerConfig/

HEADERS			:=	PollManager.hpp	ServerMonitor.hpp	ServerSocket.hpp	\
					WebServer.hpp

INCLUDES		:=	-I.	-I$(MESSAGE_DIR) -I$(CONFIG_DIR)

HEADERS		:=	Autoindex_namespace.hpp

FILES		:=	Autoindex

CXX			:=	c++
RM			:=	rm

SRCS		=	$(addsuffix .cpp, $(FILES))

OBJS		=	$(SRCS:.cpp=.o)
CXXFLAGS	=	-Wall -Wextra -Werror -g3 -std=c++98 -fPIC

all: $(NAME)

$(NAME): $(HEADERS) $(OBJS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $(OBJS)
#$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	$(RM) -rf $(OBJS)

fclean:	clean
	$(RM) -rf $(NAME)

re:		fclean all

.PHONY: all clean fclean re
