# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/02 14:37:00 by jyao              #+#    #+#              #
#    Updated: 2023/12/13 09:48:05 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile -*-

# REMEMBER TO REMOVE REMOVE REMOVE REMOVE REMOVE TEST FILES.CPP FROM LIBS!!!!!

CXX			:=	c++
# CXXFLAGS	:=	-Wall -Wextra -Werror -fsanitize=address -g3 -std=c++98 -fPIC
CXXFLAGS	:=	-Wall -Wextra -Werror -g3 -std=c++98 -fPIC


UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
LIB_SUFFIX	:=	.so
LIB_COMPILE	:=	$(CXX) $(CXXFLAGS) -shared -o
endif
ifeq ($(UNAME), Darwin)
LIB_SUFFIX	:=	.a
LIB_COMPILE	:=	ar -rvs
endif

NAME		=	$(addsuffix $(LIB_SUFFIX), libconfparser)
# NAME		:=	parserTEST
HTTP_DIR	:=	../http/
AUTOINDEX_DIR	=	$(HTTP_DIR)Autoindex
MESSAGE_DIR	=	$(HTTP_DIR)Message/

INCLUDES	:=	-I. -I$(MESSAGE_DIR) -I$(HTTP_DIR) -I$(AUTOINDEX_DIR)

HEADERS		:=	ADirective.hpp	DirectiveBlock.hpp	DirectiveSimple.hpp	\
				ServerParser_namespace.hpp				ServerConfig.hpp


RM			:=	rm
FILES		:=	ADirective	DirectiveBlock	DirectiveSimple	\
				ServerParser_namespace			ServerConfig

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