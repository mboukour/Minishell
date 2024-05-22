# NAME = minishell
# SRC = minishell.c lexer/*.c utils/*.c ast/*.c pre_tree/*.c garbage_collector/*.c
# CC = cc

# all: fclean $(NAME)

# $(NAME): 
# 	$(CC) -g $(SRC) -o $(NAME) -lreadline

# fclean:
# 	rm -f $(NAME)


NAME = minishell
SRC = minishell.c lexer/*.c utils/*.c ast/*.c pre_tree/*.c execution/*.c garbage_collector/*.c env_management/*.c builtins/export.c
LINKREADLINELIB = $(shell brew --prefix readline)/lib
LINKREADLINEINCLUDE = $(shell brew --prefix readline)/include
CC = cc

all: fclean $(NAME)

$(NAME): 
	$(CC) -I$(LINKREADLINEINCLUDE) -L$(LINKREADLINELIB) -lreadline $(SRC) -o $(NAME)

fclean:
	rm -f $(NAME)
