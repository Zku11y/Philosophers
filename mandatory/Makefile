NAME = philo
CC = cc
FLAGS = -Wall -Wextra -Werror
# FLAGS = -g -fsanitize=thread
SRC = philo.c utils.c
OBJ = $(SRC:.c=.o)

RED = \e[1;31m
GREEN = \e[1;32m
YELLOW = \e[1;33m
BLUE = \e[1;34m
CLOSE = \e[0m\n

all:$(NAME)

$(NAME):$(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)

%.o:%.c philo.h
	@printf "$(YELLOW)🛠 Compiling...$(CLOSE)"
	@$(CC) $(FLAGS) -c $< -o $@
	@printf "$(GREEN)✅ Compiled!$(CLOSE)"

clean:
	@printf "$(YELLOW)🧹 Cleaning...$(CLOSE)"
	@rm -f $(OBJ)
	@printf "$(YELLOW)✅ Cleaned$(CLOSE)"

fclean: clean
	@printf "$(YELLOW)🧼 Super Cleaning...$(CLOSE)"
	@rm -f $(NAME)
	@printf "$(YELLOW)✅ Super Cleaned$(CLOSE)"

re: fclean all

.PHONY: clean