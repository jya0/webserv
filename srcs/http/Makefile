# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/13 18:31:50 by jyao              #+#    #+#              #
#    Updated: 2023/11/19 14:14:23 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile -*-

NAME		:=	libhttp.a

# LIB			=	./lib/
HEADERS		:=	AMessage.hpp	Header.hpp	Http_namespace.hpp	\
				Request.hpp		Response.hpp
INCLUDES	:=	-I../inc/
CC			=	c++
FILES		:=	AMessage	Header	Request	Response
SRCS		=	$(addsuffix .cpp, $(FILES))

# srcs/cgi/
# srcs/http/
# srcs/parser/
# srcs/socket/

OBJS		=	${SRCS:.cpp=.o}
CFLAGS		=	-Wall -Wextra -Werror -g3 -std=c++98

all: $(NAME)

$(NAME): $(HEADERS) $(OBJS)
		ar -rcs $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

# $(CC) $(CFLAGS) -I ../inc/webserv.h  $(OBJS) -o $(NAME)

# $(LIB):
# 		#make -C ./lib/

clean:
		rm -rf $(OBJS)
fclean: clean
		rm -rf ${NAME}

re: fclean all

.PHONY: all clean fclean re