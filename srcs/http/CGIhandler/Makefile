# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 17:00:46 by jyao              #+#    #+#              #
#    Updated: 2023/12/12 21:28:39 by rriyas           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile CGI -*-

CXX			:=	c++
CXXFLAGS	:=	-Wall -Wextra -Werror -fsanitize=address -g3 -std=c++98 -fPIC

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
LIB_SUFFIX	:=	.so
LIB_COMPILE	:=	$(CXX) $(CXXFLAGS) -shared -o
endif
ifeq ($(UNAME), Darwin)
LIB_SUFFIX	:=	.a
LIB_COMPILE	:=	ar -rvs
endif

NAME		=	$(addsuffix $(LIB_SUFFIX), libcgi)
# NAME		:=	cgiTest

HTTP_DIR	:=	../
MESSAGE_DIR	:=	../Message/
CONFIG_DIR	:=	../../ServerConfig/

INCLUDES	:=	-I.	-I$(MESSAGE_DIR) -I$(CONFIG_DIR) -I$(HTTP_DIR)

HEADERS		:=	CGIhandler.hpp

FILES		:=	CGIhandler
RM			:=	rm

SRCS		=	$(addsuffix .cpp, $(FILES))

OBJS		=	$(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(HEADERS) $(OBJS)
	$(LIB_COMPILE) $@ $(OBJS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES)  -o $@ -c $<

clean:
	$(RM) -rf $(OBJS)

fclean:	clean
	$(RM) -rf $(NAME)

re:		fclean all

.PHONY: all clean fclean re