# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/02 14:37:00 by jyao              #+#    #+#              #
#    Updated: 2023/09/02 14:37:23 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile -*-

NAME		:=	libconfparser.a

# LIB			=	./lib/
CC			=	c++
RM			=	rm
SRCS		=	srcs/main.cpp srcs/server/server.cpp
# srcs/cgi/
# srcs/http/
# srcs/parser/
# srcs/socket/

OBJS		=	${SRCS:.cpp=.o}
CFLAGS		=	-Wall -Wextra -Werror -g3 -std=c++98

all:	$(NAME)

$(NAME):	$(OBJS)
		$(CC) $(CFLAGS) -I ../inc/webserv.h  $(OBJS) -o $(NAME)

# $(LIB):
# 		#make -C ./lib/
clean:
		${RM} -rf $(OBJS)
fclean:	clean
		${RM} -rf ${NAME}
re:		fclean all

.PHONY: all clean fclean re