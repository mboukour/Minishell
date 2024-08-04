CC = cc
NAME = minishell

LEXER_DIR = lexer
UTILS_DIR = utils
AST_DIR = ast
PRE_TREE_DIR = pre_tree
GC_DIR = anti_leaks
ENV_DIR = env_management
BUILTINS_DIR = builtins
EXCUTION_DIR = execution

LEXER_SRC = lex_input_checker.c lexer_input_checker_utils.c linked_list_lexer.c open_checker.c tokenizer.c tokenizer_utils.c on_error_here_doc.c quotes_handler.c
UTILS_SRC = better_access.c char_utils.c error_utils.c ft_atoi.c ft_itoa.c ft_split.c ft_split_ws.c ft_strjoin.c str_utils.c str_utils2.c str_utils3.c general_utils.c g_env.c
AST_SRC = ast_utils.c ast_utils_2.c ruined_tree.c
PRE_TREE_SRC = make_middle.c linked_list_cmd_arg.c redir_handlers.c linked_list_middle.c valid_here_doc.c pre_tree_utils/arg_utils.c pre_tree_utils/arg_utils2.c pre_tree_utils/makers.c pre_tree_utils/little_utils.c
GC_SRC = cgc.c linked_list_utils.c keep_track.c fdc.c linked_list_utils_2.c
ENV_SRC = ultimate_expander.c specefic_expanders.c general_expanders.c main_expanders.c linked_list_env.c general_env.c star_matching.c heredoc_tokenizer.c env_management_utils/env_utils.c env_management_utils/env_utils2.c env_management_utils/little_env_utils.c env_management_utils/little_env_utils2.c env_management_utils/little_env_utils3.c env_management_utils/ll_manip.c env_management_utils/ll_manip2.c env_management_utils/star_utils.c env_management_utils/joinables_utils.c env_management_utils/joinables_helpers.c env_management_utils/general_utils.c env_management_utils/star_makers.c
BUILTINS_SRC = cd.c echo.c env.c exit.c export.c pwd.c unset.c builtins_utils/cd_utils.c builtins_utils/cd_utils2.c builtins_utils/export_utils.c builtins_utils/export_utils2.c builtins_utils/error_utils.c
EXCUTION_SRC = execute_cmd.c get_path.c pipeline_1.c pipeline_2.c redirections_1.c redirections_2.c traverse_tree.c
MAIN = minishell.c

HEADER_LEXER = $(LEXER_DIR)/lexer.h
HEADER_PRE_TREE = $(PRE_TREE_DIR)/pre_tree.h
HEADER_AST = $(AST_DIR)/ast.h
HEADER_GC = $(GC_DIR)/cgc.h
HEADER_ENV = $(ENV_DIR)/env_management.h
HEADER_BUILTINS = $(BUILTINS_DIR)/builtins.h
HEADER_EXECUTION = $(EXCUTION_DIR)/execution.h
HEADER_MAIN = minishell.h

LEXER_FILES = $(addprefix $(LEXER_DIR)/,$(LEXER_SRC))
UTILS_FILES = $(addprefix $(UTILS_DIR)/,$(UTILS_SRC))
AST_FILES = $(addprefix $(AST_DIR)/,$(AST_SRC))
PRE_TREE_FILES = $(addprefix $(PRE_TREE_DIR)/,$(PRE_TREE_SRC))
GC_FILES = $(addprefix $(GC_DIR)/,$(GC_SRC))
ENV_FILES = $(addprefix $(ENV_DIR)/,$(ENV_SRC))
BUILTINS_FILES = $(addprefix $(BUILTINS_DIR)/,$(BUILTINS_SRC))
EXCUTION_FILES = $(addprefix $(EXCUTION_DIR)/,$(EXCUTION_SRC))

OBJ_LEXER = $(LEXER_FILES:.c=.o)
OBJ_UTILS = $(UTILS_FILES:.c=.o)
OBJ_AST = $(AST_FILES:.c=.o)
OBJ_PRE_TREE = $(PRE_TREE_FILES:.c=.o)
OBJ_GC = $(GC_FILES:.c=.o)
OBJ_ENV = $(ENV_FILES:.c=.o)
OBJ_BUILTINS = $(BUILTINS_FILES:.c=.o)
OBJ_EXCUTION = $(EXCUTION_FILES:.c=.o)
OBJ_MAIN = $(MAIN:.c=.o)

OBJ = $(OBJ_LEXER) $(OBJ_UTILS) $(OBJ_AST) $(OBJ_PRE_TREE) $(OBJ_GC) $(OBJ_ENV) $(OBJ_BUILTINS) $(OBJ_EXCUTION) $(OBJ_MAIN)

LINKREADLINELIB = $(shell brew --prefix readline)/lib
LINKREADLINEINCLUDE = $(shell brew --prefix readline)/include
RL_FLAGS = -I$(LINKREADLINEINCLUDE) -L$(LINKREADLINELIB)
CFLAGS = -I$(LINKREADLINEINCLUDE)
LDFLAGS = -L$(LINKREADLINELIB) -lreadline

all: $(NAME)

%.o: %.c $(HEADER_MAIN) $(HEADER_GC)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -g -fsanitize=address -c $< -o $@

$(NAME): $(OBJ)
	@echo "Linking $@"
	@$(CC) $(OBJ) $(LDFLAGS) -g -fsanitize=address -o $@
	@touch ~/.reset

$(OBJ_LEXER): $(HEADER_LEXER)
$(OBJ_UTILS): $(HEADER_MAIN)
$(OBJ_AST): $(HEADER_AST)
$(OBJ_PRE_TREE): $(HEADER_PRE_TREE)
$(OBJ_ENV): $(HEADER_ENV)
$(OBJ_BUILTINS): $(HEADER_BUILTINS)
$(OBJ_EXCUTION): $(HEADER_EXECUTION)

clean:
	@echo "Cleaning objects"
	@rm -f $(OBJ)

fclean: clean
	@echo "Cleaning $(NAME)"
	@rm -f $(NAME)

re: fclean all
