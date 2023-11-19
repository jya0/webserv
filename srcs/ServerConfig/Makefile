# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/02 14:37:00 by jyao              #+#    #+#              #
#    Updated: 2023/11/19 12:41:57 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile -*-

# NAME		:=	libconfparser.a
NAME		:=	parserTEST
HEADERS		:=	ADirective.hpp	DirectiveBlock.hpp	DirectiveSimple.hpp	\
				HTTPServerParser.hpp				ServerConfig.hpp

HTTP_DIR	:=	../http/

INCLUDES	:=	-I.	-I$(HTTP_DIR)


CC			:=	c++
RM			:=	rm
FILES		:=	ADirective	DirectiveBlock	DirectiveSimple	\
				HTTPServerParser			ServerConfig	\
				test
SRCS		=	$(addsuffix .cpp, $(FILES))

OBJS		=	${SRCS:.cpp=.o}
CFLAGS		=	-Wall -Wextra -Werror -g3 -std=c++98

all: $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(HEADERS) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)
#	ar -rcs $@ $^

clean:
	${RM} -rf $(OBJS)

fclean:	clean
	${RM} -rf ${NAME}

re:		fclean all

.PHONY: all clean fclean re