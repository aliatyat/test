NAME = minishell

LIBFT = ./LIBFT/libft.a

OBJ_DIR = obj_dir

SRC = execute_in.c execute_in2.c execute_out.c execute_out2.c execute_pip.c execute_pip2.c built_in.c signal_handler.c \
ft_echo.c ft_export.c ft_cd.c main.c utils.c utils2.c utils3.c ft_error.c ft_free.c redirection.c redir_handler.c redir_process.c\
env1.c env2.c env3.c env4.c env5.c pipe.c ft_split_redirections.c ft_split_pipe.c herdoc.c heredoc2.c heredoc3.c \
expand_input.c expand_input2.c expand_heredoc.c norm_cmd.c norm_cmd2.c is_unval.c

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))


CC = cc
CFLAGS = -Wall -Wextra -Werror -g
CFLAGS =  -g
all: $(NAME)

$(NAME): $(OBJ)
	make -C ./LIBFT
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) -lreadline

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	make clean -C ./LIBFT

fclean: clean
	rm -f $(NAME)
	make fclean -C ./LIBFT

re: fclean all 

.PHONY: fclean clean all re
