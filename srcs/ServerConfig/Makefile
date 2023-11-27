# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/02 14:37:00 by jyao              #+#    #+#              #
#    Updated: 2023/11/27 15:00:29 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile -*-

# NAME		:=	libconfparser.a
HTTP_DIR	:=	../http/
HTTP_LIB	=	$(HTTP_DIR)libhttp.a
LIBS		=	$(HTTP_LIB)
NAME		:=	parserTEST
HEADERS		:=	ADirective.hpp	DirectiveBlock.hpp	DirectiveSimple.hpp	\
				HTTPServerParser.hpp				ServerConfig.hpp

INCLUDES	:=	-I.	-I$(HTTP_DIR)

CXX			:=	c++
RM			:=	rm
FILES		:=	ADirective	DirectiveBlock	DirectiveSimple	\
				HTTPServerParser			ServerConfig	\
				test						$(HTTP_DIR)Request

SRCS		=	$(addsuffix .cpp, $(FILES))

OBJS		=	$(SRCS:.cpp=.o)
CXXFLAGS		=	-Wall -Wextra -Werror -g3 -std=c++98

all: $(NAME)


$(NAME): $(LIBS) $(HEADERS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBS)
#	ar -rcs $@ $^

$(LIBS):
	make -C $(HTTP_DIR)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	$(RM) -rf $(OBJS)

fclean:	clean
	$(RM) -rf $(NAME)

re:		fclean all

.PHONY: all clean fclean re