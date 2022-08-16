NAME	=	philosopher

NAME_B	=	philosopher_bonus

SRC		=	philo/philosopher.c	philo/phls_movements.c	philo/phls_utils.c	\
			philo/phls_utils2.c	philo/threads.c

SRC_B	=	philo_bonus/philosopher.c	philo_bonus/phls_movements.c	philo_bonus/phls_utils.c	\
			philo_bonus/threads.c

HEAD	=	philo/philosopher.h

HEAD_B	=	philo_bonus/philosopher.h

OBJ		=	$(SRC:.c=.o)

OBJ_B	=	$(SRC_B:.c=.o)

CFLAG	=	-Wall -Wextra -Werror

.PHONY		:	all bonus clean fclean re re_b

all			:	$(NAME)

bonus		:	$(NAME_B)

$(NAME)		:	$(OBJ) $(HEAD)
	gcc $(CFLAG) $(OBJ) -o $(NAME)

$(NAME_B)	:	$(OBJ_B) $(HEAD_B)
	gcc $(CFLAG) $(OBJ_B) -o $(NAME_B)

clean		:
	@rm -f $(OBJ) $(OBJ_B)

fclean		:	clean
	@rm -f $(NAME) $(NAME_B)

re			:	fclean all

re_b		:	fclean bonus
