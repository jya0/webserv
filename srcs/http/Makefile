# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/13 18:31:50 by jyao              #+#    #+#              #
#    Updated: 2023/12/05 23:48:33 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*- Makefile http -*-

NAME			:=	libhttp.a

HEADERS			:=	AMessage.hpp	Header.hpp	Http_namespace.hpp	\
				Request.hpp		Response.hpp
CGI_DIR			:=	./CGIhandler/
CGI_LIB			=	$(CGI_DIR)libcgi.a
AUTOINDEX_DIR	:=	./Autoindex/
AUTOINDEX_LIB	=	$(AUTOINDEX_DIR)libautoindex.a
MESSAGE_DIR		:=	./Message/
MESSAGE_LIB		=	$(MESSAGE_DIR)libmessage.a

LIBS			=	$(CGI_LIB)	$(AUTOINDEX_LIB)	$(MESSAGE_LIB)

CXX				=	c++

CXXFLAGS		=	-Wall -Wextra -Werror -g3 -std=c++98

all: $(NAME)

$(NAME): | $(LIBS)
	libtool -static -o $@ $(LIBS)

$(CGI_LIB):
	make -C $(CGI_DIR)

$(AUTOINDEX_LIB):
	make -C $(AUTOINDEX_DIR)

$(MESSAGE_LIB):
	make -C $(MESSAGE_DIR)

# $(CXX) $(CXXFLAGS) -I ../inc/webserv.h  $(OBJS) -o $(NAME)

# $(LIB):
# 		#make -C ./lib/

clean:
	make clean -C $(CGI_DIR)
	make clean -C $(AUTOINDEX_DIR)
	make clean -C $(MESSAGE_DIR)
	rm -rf $(OBJS)
fclean: clean
	make fclean -C $(CGI_DIR)
	make fclean -C $(AUTOINDEX_DIR)
	make fclean -C $(MESSAGE_DIR)
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re $(LIBS)