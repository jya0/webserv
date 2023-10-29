NAME		=	webserv

# LIB			=	./lib/
CXX			=	c++
RM			=	rm
SRCS		=	srcs/main.cpp srcs/server/PollManager.cpp srcs/server/ServerSocket.cpp\
				srcs/server/WebServer.cpp\
				srcs/http/AMessage.cpp srcs/http/Header.cpp srcs/http/Request.cpp srcs/http/Response.cpp
# srcs/cgi/
# srcs/http/
# srcs/parser/
# srcs/socket/
# srcs/ServerConfig/HTTPServerParser.cpp

OBJS		=	${SRCS:.cpp=.o}
CXXFLAGS		=	-g3 #-Wall -Wextra -Werror -g3 -std=c++98

all:	$(NAME)

.cpp: .o
	$(CXX) $(CXXFLAGS) -I ../inc/webserv.h  -g3 -cpp $< -o ${<:.cpp=.o}

$(NAME):	$(OBJS)
		$(CXX) $(CXXFLAGS) -I ../inc/webserv.h  -g3 $(OBJS) -o $(NAME)

# $(LIB):
# 		#make -C ./lib/
clean:
		${RM} -rf $(OBJS)
fclean:	clean
		${RM} -rf ${NAME}
re:		fclean all

.PHONY: all clean fclean re