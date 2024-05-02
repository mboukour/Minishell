NAME = minishell
SRC = minishell.c lexer/*.c utils/*.c
LINKREADLINELIB = $(shell brew --prefix readline)/lib
LINKREADLINEINCLUDE = $(shell brew --prefix readline)/include
CC = cc

all: fclean $(NAME)

$(NAME): 
	$(CC) -I$(LINKREADLINEINCLUDE) -L$(LINKREADLINELIB) -lreadline $(SRC) -o $(NAME)

fclean:
	rm -f $(NAME)
