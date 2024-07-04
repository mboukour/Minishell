CC = cc
NAME = minishell
LEXER = lexer/lex_input_checker.c lexer/linked_list_lexer.c lexer/quote_checker.c lexer/tokenizer.c lexer/on_error_here_doc.c
UTILS = utils/ft_atoi.c utils/ft_itoa.c utils/ft_split.c utils/ft_strchr.c utils/ft_strcmp.c utils/ft_strcpy.c utils/ft_strdup.c utils/ft_strjoin.c utils/ft_strlcpy.c utils/ft_strlen.c utils/ft_strncmp.c utils/ft_strncpy.c utils/ft_substr.c utils/print_helper.c utils/ft_isalpha.c utils/is_c_num.c utils/better_access.c utils/ft_strcasecmp.c
AST = ast/changers.c ast/linked_list_ast.c ast/ruined_tree.c
PRE_TREE = pre_tree/expander.c pre_tree/linked_list_cmd_arg.c pre_tree/linked_list_middle.c pre_tree/make_middle.c pre_tree/valid_here_doc.c
GC = memory_management/cgc.c memory_management/linked_list_utils.c memory_management/keep_track.c
ENV = env_management/env_expander.c env_management/linked_list_env.c env_management/sort_env_list.c env_management/star_matching.c env_management/star_utils.c env_management/heredoc_tokenizer.c
BUILTINS = builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c builtins/export.c builtins/pwd.c builtins/unset.c
EXCUTION = execution/execution.c execution/get_path.c execution/traverse_tree.c
SRC = minishell.c $(LEXER) $(UTILS) $(AST) $(PRE_TREE) $(GC) $(ENV) $(BUILTINS) $(EXCUTION) $(MISCELLANEOUS)
OBJ = $(SRC:.c=.o)
INCLUDE = minishell.h
FLAGS = -Wall -Wextra -Werror -g

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

re: fclean all



# FOR MAC OS
# LINKREADLINELIB = $(shell brew --prefix readline)/lib
# LINKREADLINEINCLUDE = $(shell brew --prefix readline)/include
# $(NAME): 
# 	$(CC) -I$(LINKREADLINEINCLUDE) -L$(LINKREADLINELIB) -lreadline $(SRC) -o $(NAME)

