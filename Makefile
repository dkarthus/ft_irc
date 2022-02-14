NAME = IRC-server

SRCS =		srcs/main.cpp \
			srcs/Server.cpp \
			srcs/Socket.cpp

OBJ = $(patsubst %.cpp, %.o, $(SRCS))

CC = c++

FLAGS = #-Wall -Wextra -Werror

INCLUDE =	srcs/Server.hpp \
			srcs/Socket.hpp

RM = rm -f

all: $(NAME) $(INCLUDE)

$(NAME): $(OBJ) $(INCLUDE)
			$(CC) $(FLAGS) $(OBJ) -std=c++98 -o $(NAME)

%.o : %.cpp
			$(CC) $(FLAGS) -c $< -o $@

clean:
			${RM} ${OBJ}

fclean: clean
			$(RM) -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re