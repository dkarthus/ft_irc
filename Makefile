NAME	= irc-server

FLAGS	= -Wall -Wextra -Werror -std=c++98

SRC_FLD	= srcs/

CPPS	= server.cpp main.cpp

SOURCE	= $(addprefix $(SRC_FLDR), $(CPPS))

HPPS	= server.hpp

INCLUDE	= $(addprefix $(SRC_FLDR), $(HPPS))

OBJS	= $(SOURCE:%.cpp=%.o)

.PHONY:	all clean fclean re

all: $(NAME)

$(SRC_FLD)%.o: $(SRC_FLD)%.cpp $(INCLUDE)
	clang++ $(FLAGS) -c $< -o $@ -I $(SRC_FLD)

$(NAME): $(OBJS) $(INCLUDE)
	clang++ $(FLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all