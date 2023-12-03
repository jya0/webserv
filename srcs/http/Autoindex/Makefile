# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 14:58:03 by jyao              #+#    #+#              #
#    Updated: 2023/12/03 19:01:02 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile Autoindex -*-

NAME		:=	libautoindex.a
# NAME		:=	autoIndexTest

HEADERS		:=	Autoindex_namespace.hpp

FILES		:=	Autoindex

CXX			:=	c++
RM			:=	rm

SRCS		=	$(addsuffix .cpp, $(FILES))

OBJS		=	$(SRCS:.cpp=.o)
CXXFLAGS	=	-Wall -Wextra -Werror -g3 -std=c++98

all: $(NAME)

$(NAME): $(LIBS) $(HEADERS) $(OBJS)
	ar -rvs $@ $(OBJS)
#$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	$(RM) -rf $(OBJS)

fclean:	clean
	$(RM) -rf $(NAME)

re:		fclean all

.PHONY: all clean fclean re