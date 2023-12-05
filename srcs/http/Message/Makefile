# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/13 18:31:50 by jyao              #+#    #+#              #
#    Updated: 2023/12/05 23:49:14 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile Message -*-

NAME			:=	libmessage.a

HEADERS			:=	AMessage.hpp	Header.hpp	Http_namespace.hpp	\
				Request.hpp		Response.hpp
HTTP_DIR		:=	../
CGI_DIR			:=	../CGIhandler/
CONFIG_DIR		:=	../../ServerConfig/
AUTOINDEX_DIR	:=	../Autoindex/
INCLUDES		:=	-I.	-I$(CONFIG_DIR) -I$(CGI_DIR) -I$(AUTOINDEX_DIR) -I$(HTTP_DIR)
CXX				=	c++
FILES			:=	AMessage	Header	Request	Response	MimeTypes
SRCS			=	$(addsuffix .cpp, $(FILES))

OBJS		=	${SRCS:.cpp=.o}
CXXFLAGS		=	-Wall -Wextra -Werror -g3 -std=c++98

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

# $(CXX) $(CXXFLAGS) -I ../inc/webserv.h  $(OBJS) -o $(NAME)

# $(LIB):
# 		#make -C ./lib/

clean:
		rm -rf $(OBJS)
fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re