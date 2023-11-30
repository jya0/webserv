# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 17:00:46 by jyao              #+#    #+#              #
#    Updated: 2023/11/30 13:07:23 by rriyas           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile CGI -*-

# NAME		:=	libcgi.a
NAME		:=	cgiTest

HTTP_DIR	:=	../
HTTP_LIB	=	$(HTTP_DIR)libhttp.a
CONFIG_DIR	:=	../../ServerConfig/
CONFIG_LIB	=	$(CONFIG_DIR)libconfparser.a
LIBS		=	$(HTTP_LIB) $(CONFIG_LIB)

INCLUDES	:=	-I.	-I$(HTTP_DIR) -I../../ServerConfig/

HEADERS		:=	CGIhandler.hpp

FILES		:=	CGIhandler	test

CXX			:=	c++
RM			:=	rm

SRCS		=	$(addsuffix .cpp, $(FILES))

OBJS		=	$(SRCS:.cpp=.o)
CXXFLAGS	=	-Wall -Wextra -Werror -g3 -std=c++98 -fsanitize=address

all: $(NAME)

$(NAME): $(LIBS) $(HEADERS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBS)
#	ar -rvs $@ $^

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