# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/01 16:46:34 by jyao              #+#    #+#              #
#    Updated: 2023/12/09 22:21:50 by rriyas           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
LIB_SUFFIX	:=	.so
endif
ifeq ($(UNAME), Darwin)
LIB_SUFFIX	:=	.a
endif

NAME			=	webserv

SRCS_DIR		:=	./srcs/
HTTP_DIR		:=	$(SRCS_DIR)http/
MESSAGE_DIR		=	$(HTTP_DIR)Message/
MESSAGE_LIB		=	$(addsuffix $(LIB_SUFFIX), $(MESSAGE_DIR)libmessage)
CGI_DIR			=	$(HTTP_DIR)CGIhandler/
CGI_LIB			=	$(addsuffix $(LIB_SUFFIX), $(CGI_DIR)libcgi)
AUTOINDEX_DIR	=	$(HTTP_DIR)Autoindex/
AUTOINDEX_LIB	=	$(addsuffix $(LIB_SUFFIX), $(AUTOINDEX_DIR)libautoindex)
CONFIG_DIR		=	$(SRCS_DIR)ServerConfig/
CONFIG_LIB		=	$(addsuffix $(LIB_SUFFIX), $(CONFIG_DIR)libconfparser)
SERVER_DIR		=	$(SRCS_DIR)server/
SERVER_LIB		=	$(addsuffix $(LIB_SUFFIX), $(SERVER_DIR)libserver)

LIBS			=	$(CGI_LIB) $(AUTOINDEX_LIB) $(MESSAGE_LIB) $(CONFIG_LIB) $(SERVER_LIB)

INCLUDES		=	-I$(SRCS_DIR) -I$(HTTP_DIR) -I$(CGI_DIR) -I$(AUTOINDEX_DIR) -I$(CONFIG_DIR) -I$(SERVER_DIR) -I$(MESSAGE_DIR)

CXX				=	c++
RM				=	rm

FILES			=	main

SRCS			=	$(addsuffix .cpp, $(addprefix $(SRCS_DIR), $(FILES)))

OBJS			=	$(SRCS:.cpp=.o)

CXXFLAGS		=	-g3 -Wall -Wextra -Werror -g3 -std=c++98 -fPIC -g3  -fsanitize=address

all:	$(NAME)

$(NAME): | $(LIBS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $(OBJS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

WEBSERV:
	$(CXX) $(CXXFLAGS) $(LIBS) $(OBJS) -o $(NAME)

$(CGI_LIB):
	make -C $(CGI_DIR)

$(AUTOINDEX_LIB):
	make -C $(AUTOINDEX_DIR)

$(MESSAGE_LIB):
	make -C $(MESSAGE_DIR)

$(CONFIG_LIB):
	make -C $(CONFIG_DIR)

$(SERVER_LIB):
	make -C $(SERVER_DIR)

clean:
	make clean -C $(MESSAGE_DIR)
	make clean -C $(CGI_DIR)
	make clean -C $(AUTOINDEX_DIR)
	make clean -C $(CONFIG_DIR)
	make clean -C $(SERVER_DIR)
	$(RM) -rf $(OBJS)

fclean:	clean
	make fclean -C $(MESSAGE_DIR)
	make fclean -C $(CGI_DIR)
	make fclean -C $(AUTOINDEX_DIR)
	make fclean -C $(CONFIG_DIR)
	make fclean -C $(SERVER_DIR)
	$(RM) -rf $(NAME)

re:		fclean all

.PHONY: all clean fclean re WEBSERV $(LIBS)

# c++  -std=c++98 -Isrcs/ServerConfig -Isrcs/server -Isrcs/http/Autoindex -Isrcs/http/CGIhandler -Isrcs/http/Message -Isrcs/http  srcs/*.cpp srcs/ServerConfig/*.cpp srcs/http/Autoindex/*.cpp srcs/http/CGIhandler/*.cpp srcs/http/Message/*.cpp srcs/server/*.cpp -std=c++98