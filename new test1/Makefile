# NAME = cub3d
# CC = cc
# OBJ = $(SRC:.c=.o)
# CFLAGS = -g

# LIBFT = ./libft/libft.a

# LFLAGS = -L./minilibx/mlx -lmlx -lXext -lX11 -lm -lz
# #INCLUDES = includes/mlx/libmlx.a
# SRC = src/main.c src/player.c

# all: $(NAME)

# #mlx:
# #	make -C ./includes/mlx

# $(NAME): $(OBJ)
# 	$(CC) $(CFLAGS) $(SRC) -o $(NAME) $(LIBFT) $(LFLAGS) -g


# $(LIBFT):
# 	$(MAKE) -C ./libft


# fclean:
# 	rm -rf $(OBJ)
# 	rm -rf $(NAME)

NAME = cub3d
CC = cc
CFLAGS = -g -I./includes -I./minilibx
LDFLAGS = -L./minilibx -lmlx -lX11 -lXext -lm -L./libft -lft

SRC = fuck.c
OBJ = $(SRC:.c=.o)

LIBFT = ./libft/libft.a

all: $(LIBFT) minilibx $(NAME)

$(LIBFT):
	$(MAKE) -C ./libft

minilibx:
	@if [ ! -f ./minilibx/libmlx.a ]; then \
		echo "Building MiniLibX..."; \
		$(MAKE) -C ./minilibx; \
	fi

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	$(MAKE) -C ./libft clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C ./libft fclean
	$(MAKE) -C ./minilibx clean

re: fclean all

.PHONY: all clean fclean re minilibx