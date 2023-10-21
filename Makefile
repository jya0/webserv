NAME		=	webserv

# LIB			=	./lib/
CC			=	c++
RM			=	rm
SRCS		=	srcs/main.cpp srcs/ServerSocket.cpp\
				srcs/server/WebServer.cpp\
				srcs/http/AMessage.cpp srcs/http/Header.cpp srcs/http/Request.cpp srcs/http/Response.cpp
# srcs/cgi/
# srcs/http/
# srcs/parser/
# srcs/socket/
# srcs/ServerConfig/HTTPServerParser.cpp

OBJS		=	${SRCS:.cpp=.o}
CFLAGS		=	-g3 -Wall -Wextra -Werror -g3 -std=c++98

all:	$(NAME)

.cpp: .o
	$(CC) $(CFLAGS) -I ../inc/webserv.h  -g3 -cpp $< -o ${<:.cpp=.o}

$(NAME):	$(OBJS)
		$(CC) $(CFLAGS) -I ../inc/webserv.h  -g3 $(OBJS) -o $(NAME)

# $(LIB):
# 		#make -C ./lib/
clean:
		${RM} -rf $(OBJS)
fclean:	clean
		${RM} -rf ${NAME}
re:		fclean all

.PHONY: all clean fclean re