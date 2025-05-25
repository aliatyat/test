NAME = game 
CC = cc
OBJ = $(SRC:.c=.o)
CFLAGS = -g

LIBFT = ./libft/libft.a

LFLAGS = -L./includes/mlx -lmlx -lXext -lX11 -lm -lz
#INCLUDES = includes/mlx/libmlx.a
SRC = src/main.c src/player.c

all: $(NAME)

#mlx:
#	make -C ./includes/mlx

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME) $(LIBFT) $(LFLAGS) -g


$(LIBFT):
	$(MAKE) -C ./libft


fclean:
	rm -rf $(OBJ)
	rm -rf $(NAME)
