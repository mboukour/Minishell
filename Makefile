CC = cc
NAME = minishell
LEXER = lexer/lex_input_checker.c lexer/lexer_input_checker_utils.c lexer/linked_list_lexer.c lexer/open_checker.c lexer/tokenizer.c lexer/tokenizer_utils.c lexer/on_error_here_doc.c lexer/quotes_handler.c
UTILS = utils/better_access.c utils/char_utils.c utils/error_utils.c utils/ft_atoi.c utils/ft_itoa.c utils/ft_split.c utils/ft_split_ws.c utils/ft_strjoin.c utils/str_utils.c utils/str_utils2.c utils/str_utils3.c utils/general_utils.c utils/g_env.c
AST = ast/ast_utils.c ast/ast_utils_2.c ast/ruined_tree.c
PRE_TREE = pre_tree/make_middle.c pre_tree/linked_list_cmd_arg.c pre_tree/redir_handlers.c pre_tree/linked_list_middle.c pre_tree/valid_here_doc.c pre_tree/pre_tree_utils/arg_utils.c pre_tree/pre_tree_utils/arg_utils2.c pre_tree/pre_tree_utils/makers.c pre_tree/pre_tree_utils/little_utils.c
GC = anti_leaks/cgc.c anti_leaks/linked_list_utils.c anti_leaks/keep_track.c anti_leaks/fdc.c anti_leaks/linked_list_utils_2.c
ENV =  env_management/ultimate_expander.c  env_management/specefic_expanders.c env_management/general_expanders.c env_management/main_expanders.c env_management/linked_list_env.c env_management/sort_env_list.c env_management/star_matching.c env_management/heredoc_tokenizer.c env_management/env_management_utils/env_utils.c env_management/env_management_utils/env_utils2.c env_management/env_management_utils/little_env_utils.c env_management/env_management_utils/little_env_utils2.c env_management/env_management_utils/little_env_utils3.c env_management/env_management_utils/ll_manip.c env_management/env_management_utils/ll_manip2.c env_management/env_management_utils/star_utils.c env_management/env_management_utils/joinables_utils.c env_management/env_management_utils/joinables_helpers.c env_management/env_management_utils/general_utils.c env_management/env_management_utils/star_makers.c
BUILTINS = builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c builtins/export.c builtins/pwd.c builtins/unset.c builtins/builtins_utils/cd_utils.c builtins/builtins_utils/cd_utils2.c builtins/builtins_utils/export_utils.c builtins/builtins_utils/export_utils2.c builtins/builtins_utils/error_utils.c
EXCUTION = execution/execute_cmd.c execution/get_path.c execution/pipeline_1.c execution/pipeline_2.c execution/redirections_1.c execution/redirections_2.c execution/traverse_tree.c
SRC = debug.c minishell.c $(LEXER) $(UTILS) $(AST) $(PRE_TREE) $(GC) $(ENV) $(BUILTINS) $(EXCUTION)
OBJ = $(SRC:.c=.o)
INCLUDE = minishell.h
LINKREADLINELIB = $(shell brew --prefix readline)/lib
LINKREADLINEINCLUDE = $(shell brew --prefix readline)/include
RL_FLAGS = -I$(LINKREADLINEINCLUDE) -L$(LINKREADLINELIB)
CFLAGS = -I$(LINKREADLINEINCLUDE)
LDFLAGS = -L$(LINKREADLINELIB) -lreadline -g

all: $(NAME)

%.o: %.c $(INCLUDE)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -g -c $< -o $@

$(NAME): $(OBJ)
	@echo "Linking $@"
	@$(CC) $(OBJ) $(LDFLAGS) -o $@

clean:
	@echo "Cleaning objects"
	@rm -f $(OBJ)

fclean: clean
	@echo "Cleaning $(NAME)"
	@rm -f $(NAME)

re: fclean all
