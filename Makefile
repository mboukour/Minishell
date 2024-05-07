NAME = minishell
SRC = minishell.c lexer/*.c utils/*.c ast/*.c pre_tree/*.c
CC = cc

all: fclean $(NAME)

$(NAME): 
	$(CC) -g $(SRC) -o $(NAME) -lreadline

fclean:
	rm -f $(NAME)
