CC = cc
NAME = philo
BNAME = philo_bonus
FLAGS = -Wall -Wextra -Werror
SRC = mandatory/philo.c \
	  mandatory/utils.c \
	  mandatory/actions.c \
	  mandatory/init.c
BSRC = bonus/philo_bonus.c bonus/utils_bonus.c
OBJ = $(SRC:.c=.o)
BOBJ = $(BSRC:.c=.o)

RED = \e[1;31m
GREEN = \e[1;32m
YELLOW = \e[1;33m
BLUE = \e[1;34m
CLOSE = \e[0m\n

all:$(NAME)

bonus:$(BNAME)

$(NAME):$(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)

$(BNAME):$(BOBJ)
	@$(CC) $(FLAGS) $(BOBJ) -o $(BNAME)

bonus/%.o:bonus/%.c bonus/philo_bonus.h
	@printf "$(YELLOW)🛠 Compiling...$(CLOSE)"
	@$(CC) $(FLAGS) -c $< -o $@
	@printf "$(GREEN)✅ Compiled!$(CLOSE)"

mandatory/%.o:mandatory/%.c mandatory/philo.h
	@printf "$(YELLOW)🛠 Compiling...$(CLOSE)"
	@$(CC) $(FLAGS) -c $< -o $@
	@printf "$(GREEN)✅ Compiled!$(CLOSE)"


clean:
	@printf "$(YELLOW)🧹 Cleaning...$(CLOSE)"
	@rm -f $(OBJ) $(BOBJ)
	@printf "$(YELLOW)✅ Cleaned$(CLOSE)"

fclean: clean
	@printf "$(YELLOW)🧼 Super Cleaning...$(CLOSE)"
	@rm -f $(NAME) $(BNAME)
	@printf "$(YELLOW)✅ Super Cleaned$(CLOSE)"

re: fclean all

.PHONY: clean