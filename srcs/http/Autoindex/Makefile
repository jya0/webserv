# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 14:58:03 by jyao              #+#    #+#              #
#    Updated: 2023/12/05 23:49:48 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile Autoindex -*-

NAME		:=	libautoindex.a
# NAME		:=	autoIndexTest

MESSAGE_DIR		:=	../Message/
CONFIG_DIR		:=	../../ServerConfig/

HEADERS			:=	PollManager.hpp	ServerMonitor.hpp	ServerSocket.hpp	\
					WebServer.hpp

INCLUDES		:=	-I.	-I$(MESSAGE_DIR) -I$(CONFIG_DIR)

HEADERS		:=	Autoindex_namespace.hpp

FILES		:=	Autoindex

CXX			:=	c++
RM			:=	rm

SRCS		=	$(addsuffix .cpp, $(FILES))

OBJS		=	$(SRCS:.cpp=.o)
CXXFLAGS	=	-Wall -Wextra -Werror -g3 -std=c++98

DEPDIR := .deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

all: $(NAME)

$(NAME): $(HEADERS) $(OBJS)
	libtool -static -v -o $@ $(OBJS)
#$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBS)



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