NAME = philo

SRC = philo.c \
      philo_sync.c \
      philo_utils.c \
      utils.c \
      init.c

OBJ = $(SRC:.c=.o)
CC = cc
CFLAGS = -Wall -Werror -Wextra -pthread
RM = rm -f

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all
