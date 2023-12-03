# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 17:00:46 by jyao              #+#    #+#              #
#    Updated: 2023/12/03 14:03:20 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile CGI -*-

NAME		:=	libcgi.a
# NAME		:=	cgiTest

HTTP_DIR	:=	../
HTTP_LIB	=	$(HTTP_DIR)libhttp.a
CONFIG_DIR	:=	../../ServerConfig/
CONFIG_LIB	=	$(CONFIG_DIR)libconfparser.a
LIBS		=	$(HTTP_LIB) $(CONFIG_LIB)

INCLUDES	:=	-I.	-I$(HTTP_DIR) -I$(CONFIG_DIR)

HEADERS		:=	CGIhandler.hpp

FILES		:=	CGIhandler

CXX			:=	c++
RM			:=	rm

SRCS		=	$(addsuffix .cpp, $(FILES))

OBJS		=	$(SRCS:.cpp=.o)
CXXFLAGS	=	-Wall -Wextra -Werror -g3 -std=c++98 -fsanitize=address

all: $(NAME)

$(NAME): $(LIBS) $(HEADERS) $(OBJS)
	ar -rvs $@ $(OBJS)

$(LIBS):
	make -C $(HTTP_DIR)
	make -C $(CONFIG_DIR)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	$(RM) -rf $(OBJS)

fclean:	clean
	$(RM) -rf $(NAME)

re:		fclean all

.PHONY: all clean fclean re