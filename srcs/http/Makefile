# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/13 18:31:50 by jyao              #+#    #+#              #
#    Updated: 2023/08/13 18:39:44 by rriyas           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile -*-

NAME		:=	libhttp.a

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