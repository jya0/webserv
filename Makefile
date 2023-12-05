# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/01 16:46:34 by jyao              #+#    #+#              #
#    Updated: 2023/12/05 23:50:16 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	webserv

SRCS_DIR		:=	./srcs/
HTTP_DIR		=	$(SRCS_DIR)http/
HTTP_LIB		=	$(HTTP_DIR)libhttp.a
MESSAGE_DIR		=	$(HTTP_DIR)Message/
CGI_DIR			=	$(HTTP_DIR)CGIhandler/
AUTOINDEX_DIR	=	$(HTTP_DIR)Autoindex/
CONFIG_DIR		=	$(SRCS_DIR)ServerConfig/
CONFIG_LIB		=	$(CONFIG_DIR)libconfparser.a
SERVER_DIR		=	$(SRCS_DIR)server/
SERVER_LIB		=	$(SERVER_DIR)libserver.a

LIBS			=	$(HTTP_LIB) $(CONFIG_LIB) $(SERVER_LIB)

INCLUDES		:=	-I. -I$(HTTP_DIR) -I$(CGI_DIR) -I$(AUTOINDEX_DIR) -I$(CONFIG_DIR) -I$(SERVER_DIR) -I$(MESSAGE_DIR)

CXX				=	c++
RM				=	rm

FILES			=	main

SRCS			=	$(addsuffix .cpp, $(addprefix $(SRCS_DIR), $(FILES)))

OBJS			=	$(SRCS:.cpp=.o)

CXXFLAGS		=	-g3 -fsanitize=address -Wall -Wextra -Werror -g3 -std=c++98

DEPDIR := .deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

all:	$(NAME)

%.o: %.cpp #$(DEPDIR)/%.d | $(DEPDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES)  -o $@ -c $<

#$(DEPDIR): ; @mkdir -p $@

#DEPFILES := $(SRCS:%.c=$(DEPDIR)/%.d)
#$(DEPFILES):

#include $(wildcard $(DEPFILES))

$(NAME):	$(HEADERS) | $(LIBS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $(OBJS) -o $@

$(HTTP_LIB):
	make -C $(HTTP_DIR)

$(CGI_LIB):
	make -C $(CGI_DIR)

$(AUTOINDEX_LIB):
	make -C $(AUTOINDEX_DIR)

$(CONFIG_LIB):
	make -C $(CONFIG_DIR)

$(SERVER_LIB):
	make -C $(SERVER_DIR)

clean:
	make clean -C $(HTTP_DIR)
	make clean -C $(CGI_DIR)
	make clean -C $(AUTOINDEX_DIR)
	make clean -C $(CONFIG_DIR)
	make clean -C $(SERVER_DIR)
	$(RM) -rf $(OBJS)

fclean:	clean
	make fclean -C $(HTTP_DIR)
	make fclean -C $(CGI_DIR)
	make fclean -C $(AUTOINDEX_DIR)
	make fclean -C $(CONFIG_DIR)
	make fclean -C $(SERVER_DIR)
	$(RM) -rf $(NAME)

re:		fclean all

.PHONY: all clean fclean re $(LIBS)