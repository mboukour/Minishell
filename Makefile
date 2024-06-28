CC = cc
NAME = minishell
LEXER = lexer/lex_input_checker.c lexer/linked_list_lexer.c lexer/quote_checker.c lexer/tokenizer.c
UTILS = utils/ft_atoi.c utils/ft_itoa.c utils/ft_split.c utils/ft_strchr.c utils/ft_strcmp.c utils/ft_strcpy.c utils/ft_strdup.c utils/ft_strjoin.c utils/ft_strlcpy.c utils/ft_strlen.c utils/ft_strncmp.c utils/ft_strncpy.c utils/ft_substr.c utils/print_helper.c utils/ft_isalpha.c
AST = ast/changers.c ast/linked_list_ast.c ast/ruined_tree.c
PRE_TREE = pre_tree/expander.c pre_tree/linked_list_cmd_arg.c pre_tree/linked_list_middle.c pre_tree/make_middle.c
GC = garbage_collector/cgc.c garbage_collector/linked_list_utils.c
ENV = env_management/env_expander.c env_management/linked_list_env.c env_management/sort_env_list.c env_management/star_matching.c env_management/star_utils.c
BUILTINS = builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c builtins/export.c builtins/pwd.c builtins/unset.c
EXCUTION = execution/execution.c execution/get_path.c execution/traverse_tree.c
SRC = minishell.c $(LEXER) $(UTILS) $(AST) $(PRE_TREE) $(GC) $(ENV) $(BUILTINS) $(EXCUTION)
OBJ = $(SRC:.c=.o)
INCLUDE = minishell.h
FLAGS = -g

all: $(NAME)


%.o: %.c $(INCLUDE)
	@echo "Compiling $<"
	@$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@echo "Linking $@"
	@$(CC) $^ -o $@ -lreadline

clean:
	@echo "Cleaning objects"
	@rm -f $(OBJ)

fclean: clean
	@echo "Cleaning $(NAME)"
	@rm -f $(NAME)


# NAME = minishell
# SRC = minishell.c lexer/*.c utils/*.c ast/*.c pre_tree/*.c garbage_collector/*.c env_management/*.c builtins/*.c execution/*.c
# LINKREADLINELIB = $(shell brew --prefix readline)/lib
# LINKREADLINEINCLUDE = $(shell brew --prefix readline)/include
# CC = cc

# all: fclean $(NAME)

# $(NAME): 
# 	$(CC) -I$(LINKREADLINEINCLUDE) -L$(LINKREADLINELIB) -lreadline $(SRC) -o $(NAME)

# fclean:
# 	rm -f $(NAME)
