# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 17:00:46 by jyao              #+#    #+#              #
#    Updated: 2023/12/14 09:02:16 by rriyas           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile CGI -*-

CXX			:=	c++
# CXXFLAGS	:=	-Wall -Wextra -Werror -g3 -std=c++98
CXXFLAGS	:=	-Wall -Wextra -Werror -g3 -fsanitize=address -std=c++98
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

NAME		=	$(addsuffix $(LIB_SUFFIX), libcgi)
# NAME		:=	cgiTest

HTTP_DIR	:=	../
MESSAGE_DIR	:=	../Message/
CONFIG_DIR	:=	../../ServerConfig/
SERVER_DIR	:=	../../server/

INCLUDES	:=	-I.	-I$(MESSAGE_DIR) -I$(CONFIG_DIR) -I$(HTTP_DIR) -I$(SERVER_DIR)

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