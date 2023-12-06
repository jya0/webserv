# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 17:00:46 by jyao              #+#    #+#              #
#    Updated: 2023/12/06 13:22:49 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile CGI -*-

NAME		:=	libcgi.so
# NAME		:=	cgiTest

HTTP_DIR	:=	../
MESSAGE_DIR	:=	../Message/
CONFIG_DIR	:=	../../ServerConfig/

INCLUDES	:=	-I.	-I$(MESSAGE_DIR) -I$(CONFIG_DIR) -I$(HTTP_DIR)

HEADERS		:=	CGIhandler.hpp

FILES		:=	CGIhandler

CXX			:=	c++
RM			:=	rm

SRCS		=	$(addsuffix .cpp, $(FILES))

OBJS		=	$(SRCS:.cpp=.o)
CXXFLAGS	=	-Wall -Wextra -Werror -g3 -fsanitize=address -std=c++98 -fPIC

all: $(NAME)

$(NAME): $(HEADERS) $(OBJS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $(OBJS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES)  -o $@ -c $<

clean:
	$(RM) -rf $(OBJS)

fclean:	clean
	$(RM) -rf $(NAME)

re:		fclean all

.PHONY: all clean fclean re