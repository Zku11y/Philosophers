NAME = philo_bonus
CC = cc
CFLAGS = -Wall -Wextra -Werror 
SRC = philo.c utils.c parse.c simulation.c
OBJ = $(SRC:.c=.o)
DEPS = $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c philo_bonus.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean
