NAME = philo

SRC = main.c \
      src/philo_sync.c \
      src/philo_utils.c \
      src/utils.c \
      src/init.c

OBJ_DIR = objs
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
CC = cc
CFLAGS = -Wall -Werror -Wextra -pthread
RM = rm -f

# Crear directorios necesarios para los archivos .o
$(OBJ_DIR)/%.o: %.c philo.h
	mkdir -p $(@D) # Asegura que el directorio exista
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	$(RM) $(OBJ)
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all
