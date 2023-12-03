# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/03 13:29:44 by jyao              #+#    #+#              #
#    Updated: 2023/12/03 16:16:52 by rriyas           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile server

NAME			=	libserver.a

HTTP_DIR		:=	../http/
HTTP_LIB		=	$(HTTP_DIR)libhttp.a
CGI_DIR			:=	../http/CGIhandler/
CGI_LIB			=	$(CGI_DIR)libcgi.a
AUTOINDEX_DIR	:=	../http/Autoindex/
AUTOINDEX_LIB	=	$(HTTP_DIR)libautoindex.a
CONFIG_DIR		:=	../ServerConfig/
CONFIG_LIB		=	$(CONFIG_DIR)libconfparser.a

HEADERS			:=	PollManager.hpp	ServerMonitor.hpp	ServerSocket.hpp	\
					WebServer.hpp

LIBS			=	$(HTTP_LIB) $(CGI_LIB) $(AUTOINDEX_LIB) $(CONFIG_LIB)

INCLUDES		:=	-I.	-I$(HTTP_DIR) -I$(CGI_DIR) -I$(AUTOINDEX_DIR) -I$(CONFIG_DIR)

CXX				=	c++
RM				=	rm

FILES			:=	PollManager		ServerSocket	\
					WebServer		ServerMonitor

SRCS			=	$(addsuffix .cpp, $(FILES))

OBJS			=	$(addsuffix .o, $(FILES))

CXXFLAGS		=	-g3 -fsanitize=address -Wall -Wextra -Werror -g3 -std=c++98

all:	$(NAME)


$(NAME):	$(HEADER) $(OBJS)
	ar -rvs $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	$(RM) -rf $(OBJS)

fclean:	clean
	$(RM) -rf $(NAME)

re:		fclean all

.PHONY: all clean fclean re