CC = cc
NAME = minishell
LEXER = lexer/lex_input_checker.c lexer/lexer_input_checker_utils.c lexer/linked_list_lexer.c lexer/open_checker.c lexer/tokenizer.c lexer/tokenizer_utils.c lexer/on_error_here_doc.c lexer/quotes_handler.c
UTILS = utils/better_access.c utils/char_utils.c utils/error_utils.c utils/ft_atoi.c utils/ft_itoa.c utils/ft_split.c utils/ft_split_ws.c utils/ft_strjoin.c utils/str_utils.c utils/str_utils2.c utils/str_utils3.c utils/general_utils.c
AST = ast/ast_utils.c ast/ast_utils_2.c ast/ruined_tree.c
PRE_TREE = pre_tree/make_middle.c pre_tree/linked_list_cmd_arg.c pre_tree/redir_handlers.c pre_tree/linked_list_middle.c pre_tree/valid_here_doc.c pre_tree/pre_tree_utils/arg_utils.c pre_tree/pre_tree_utils/arg_utils2.c pre_tree/pre_tree_utils/makers.c pre_tree/pre_tree_utils/little_utils.c
GC = anti_leaks/cgc.c anti_leaks/linked_list_utils.c anti_leaks/keep_track.c anti_leaks/fdc.c anti_leaks/linked_list_utils_2.c
ENV = env_management/env_expander.c env_management/linked_list_env.c env_management/sort_env_list.c env_management/star_matching.c env_management/star_utils.c env_management/heredoc_tokenizer.c
BUILTINS = builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c builtins/export.c builtins/pwd.c builtins/unset.c builtins/builtins_utils/cd_utils.c builtins/builtins_utils/cd_utils2.c builtins/builtins_utils/export_utils.c builtins/builtins_utils/export_utils2.c builtins/builtins_utils/error_utils.c
EXCUTION = execution/execution.c execution/get_path.c execution/traverse_tree.c
SRC = minishell.c $(LEXER) $(UTILS) $(AST) $(PRE_TREE) $(GC) $(ENV) $(BUILTINS) $(EXCUTION) $(MISCELLANEOUS)
OBJ = $(SRC:.c=.o)
INCLUDE = minishell.h
FLAGS = -Wall -Wextra -Werror

all: fclean $(NAME)

LINKREADLINELIB = $(shell brew --prefix readline)/lib
LINKREADLINEINCLUDE = $(shell brew --prefix readline)/include
$(NAME): 
	$(CC) -I$(LINKREADLINEINCLUDE) -L$(LINKREADLINELIB) -lreadline $(SRC) -o $(NAME)

# %.o: %.c $(INCLUDE)
# 	@echo "Compiling $<"
# 	@$(CC) $(FLAGS) -c $< -o $@

# $(NAME): $(OBJ)
# 	@echo "Linking $@"
# 	@$(CC) $^ -o $@ -lreadline

clean:
	@echo "Cleaning objects"
	@rm -f $(OBJ)

fclean: clean
	@echo "Cleaning $(NAME)"
	@rm -f $(NAME)

re: fclean all



# FOR MAC OS

