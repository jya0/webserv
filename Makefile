# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyao <jyao@student.42abudhabi.ae>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/01 16:46:34 by jyao              #+#    #+#              #
#    Updated: 2023/12/06 13:33:49 by jyao             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	webserv

SRCS_DIR		:=	./srcs/
HTTP_DIR		:=	$(SRCS_DIR)http/
MESSAGE_DIR		=	$(HTTP_DIR)Message/
MESSAGE_LIB		=	$(MESSAGE_DIR)libmessage.so
CGI_DIR			=	$(HTTP_DIR)CGIhandler/
CGI_LIB			=	$(CGI_DIR)libcgi.so
AUTOINDEX_DIR	=	$(HTTP_DIR)Autoindex/
AUTOINDEX_LIB	=	$(AUTOINDEX_DIR)libautoindex.so
CONFIG_DIR		=	$(SRCS_DIR)ServerConfig/
CONFIG_LIB		=	$(CONFIG_DIR)libconfparser.so
SERVER_DIR		=	$(SRCS_DIR)server/
SERVER_LIB		=	$(SERVER_DIR)libserver.so

LIBS			=	$(MESSAGE_LIB) $(CGI_LIB) $(AUTOINDEX_LIB) $(CONFIG_LIB) $(SERVER_LIB)

INCLUDES		:=	-I$(SRCS_DIR) -I$(HTTP_DIR) -I$(CGI_DIR) -I$(AUTOINDEX_DIR) -I$(CONFIG_DIR) -I$(SERVER_DIR) -I$(MESSAGE_DIR)

CXX				=	c++
RM				=	rm

FILES			=	main

SRCS			=	$(addsuffix .cpp, $(addprefix $(SRCS_DIR), $(FILES)))

OBJS			=	$(SRCS:.cpp=.o)

CXXFLAGS		=	-g3 -fsanitize=address -Wall -Wextra -Werror -g3 -std=c++98 -fPIC

all:	$(NAME)

%.o: %.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(NAME): | $(LIBS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $(OBJS) -o $@
#c++ -shared -o libwebserv.so $(MESSAGE_DIR)*.o $(CGI_DIR)*.o $(AUTOINDEX_DIR)*.o $(CONFIG_DIR)*.o $(SERVER_DIR)*.o

$(MESSAGE_LIB):
	make -C $(MESSAGE_DIR)

$(CGI_LIB):
	make -C $(CGI_DIR)

$(AUTOINDEX_LIB):
	make -C $(AUTOINDEX_DIR)

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

.PHONY: all clean fclean re $(LIBS)

# c++ -std=c++98 -Isrcs/ServerConfig -Isrcs/server -Isrcs/http/Autoindex -Isrcs/http/CGIhandler -Isrcs/http/Message -Isrcs/http  srcs/*.cpp srcs/ServerConfig/*.cpp srcs/http/Autoindex/*.cpp srcs/http/CGIhandler/*.cpp srcs/http/Message/*.cpp srcs/server/*.cpp -std=c++98