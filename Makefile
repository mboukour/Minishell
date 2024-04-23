NAME = minishell
SRC = minishell.c lexer/*.c utils/*.c

all: 
	gcc $(SRC) -o minishell -g -I$(shell brew --prefix readline)/include -L$(shell brew --prefix readline)/lib -lreadline

fclean:
	rm -f $(NAME)