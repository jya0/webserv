# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/02 14:37:00 by jyao              #+#    #+#              #
#    Updated: 2023/12/06 01:21:28 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile -*-

# REMEMBER TO REMOVE REMOVE REMOVE REMOVE REMOVE TEST FILES.CPP FROM LIBS!!!!!

NAME		:=	libconfparser.a
# NAME		:=	parserTEST
HTTP_DIR	:=	../http/
MESSAGE_DIR	=	$(HTTP_DIR)Message/

INCLUDES	:=	-I. -I$(MESSAGE_DIR) -I$(HTTP_DIR)

HEADERS		:=	ADirective.hpp	DirectiveBlock.hpp	DirectiveSimple.hpp	\
				ServerParser_namespace.hpp				ServerConfig.hpp

CXX			:=	c++
RM			:=	rm
FILES		:=	ADirective	DirectiveBlock	DirectiveSimple	\
				ServerParser_namespace			ServerConfig	

SRCS		=	$(addsuffix .cpp, $(FILES))

OBJS		=	$(SRCS:.cpp=.o)
CXXFLAGS	=	-Wall -Wextra -Werror -g3 -std=c++98

all: $(NAME)

$(NAME): $(HEADERS) $(OBJS)
	ar -rvs $@ $(OBJS)


%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES)  -o $@ -c $<

clean:
	$(RM) -rf $(OBJS)

fclean:	clean
	$(RM) -rf $(NAME)

re:		fclean all

.PHONY: all clean fclean re