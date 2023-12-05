# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 17:00:46 by jyao              #+#    #+#              #
#    Updated: 2023/12/05 23:49:36 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile CGI -*-

NAME		:=	libcgi.a
# NAME		:=	cgiTest

HTTP_DIR	:=	../
MESSAGE_DIR	:=	../Message/
CONFIG_DIR	:=	../../ServerConfig/

INCLUDES	:=	-I.	-I$(MESSAGE_DIR) -I$(CONFIG_DIR) -I$(HTTP_DIR)

HEADERS		:=	CGIhandler.hpp

FILES		:=	CGIhandler

CXX			:=	c++
RM			:=	rm

SRCS		=	$(addsuffix .cpp, $(FILES))

OBJS		=	$(SRCS:.cpp=.o)
CXXFLAGS	=	-Wall -Wextra -Werror -g3 -fsanitize=address -std=c++98

DEPDIR := .deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

all: $(NAME)

$(NAME): $(HEADERS) $(OBJS)
	libtool -static -v -o $@ $(OBJS)

%.o: %.cpp #$(DEPDIR)/%.d | $(DEPDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES)  -o $@ -c $<

#$(DEPDIR): ; @mkdir -p $@

#DEPFILES := $(SRCS:%.c=$(DEPDIR)/%.d)
#$(DEPFILES):

#include $(wildcard $(DEPFILES))

clean:
	$(RM) -rf $(OBJS)

fclean:	clean
	$(RM) -rf $(NAME)

re:		fclean all

.PHONY: all clean fclean re