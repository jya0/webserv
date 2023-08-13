NAME		=	webserv

# LIB			=	./lib/
CC			=	c++
RM			=	rm
SRCS		=	srcs/main.cpp
# srcs/cgi/
# srcs/http/
# srcs/parser/
# srcs/server/
# srcs/socket/

OBJS		=	${SRCS:.cpp=.o}
CFLAGS		=	-Wall -Wextra -Werror -g3 -std=c++98

all:	$(NAME)

$(NAME):	$(OBJS)
		$(CC) $(CFLAGS) -I ../inc/webserv.h  $(OBJS) -o $(NAME)

# $(LIB):
# 		#make -C ./lib/
clean:
		${RM} -rf $(OBJS)
fclean:	clean
		${RM} -rf ${NAME}
re:		fclean all

.PHONY: all clean fclean re