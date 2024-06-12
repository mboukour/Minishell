#ifndef MINIHSELL_H
#define MINIHSELL_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <time.h>
#include <signal.h>
#include <dirent.h>
#include <limits.h>
#include "./garbage_collector/cgc.h"
#include "execution/execution.h"

# define NONE -1
# define SECURE_PATH "/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:."
# define PARSE_ERROR "Parse error near: "
# define QUOTE_ERROR "Parse error: open quotes\n"
# define PARA_ERROR "Parse error: open parantheses\n"

enum e_token
{
	WORD = -1,
	COMMAND = -1,
	WHITE_SPACE = ' ',
	NEW_LINE = '\n',
	QUOTE = '\'',
	DOUBLE_QUOTE = '\"',
	ENV = '$',
	PIPE_LINE = '|',
	REDIR_IN = '<',
	REDIR_OUT = '>',
	OPEN_PARANTHESE = '(',
	CLOSE_PARANTHESE = ')',
	STAR = '*',
	ENV_STAR,
	AND,
	OR,
	HERE_DOC,
	DREDIR_OUT,
};



enum e_state
{
	IN_DQUOTE,
	IN_QUOTE,
	IN_PARENTHESES,
	GENERAL,
} ;

enum e_treetypes
{
	OPERATOR,
	CONDITION
};

enum e_modes
{
	LEFT,
	RIGHT
};

enum e_wait_modes
{
	NO_WAIT,
	WAIT
};

enum e_malloc_types
{
	MANUAL,
	GC
};

enum e_error_codes
{
	PARSING_ERROR = 1,
	MALLOC_ERROR
};

enum e_bulitins
{
	ECHO,
	EXIT,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV_CMD,
};

enum e_replace_modes
{
	NO_REPLACE,
	ONLY_ENV,
	REPLACE_ALL
};

typedef struct s_env
{
	char *value;
	struct s_env *next;
	struct s_env *prev;
} t_env;

typedef struct s_lex
{
	char *content;
	int len;
	int to_replace;
	enum e_token token;
	enum e_state state;
	struct s_lex *prev;
	struct s_lex *next;
} t_lex;

typedef struct s_redir
{
	int token;
	int to_replace;
	char *redir_string;
	struct s_redir *next;
}	t_redir;

typedef struct s_arg
{
	char *content;
	int to_replace;
	struct s_arg *next;
}	t_arg;

typedef struct s_middle
{
	int		token;
	char	*content;
	t_arg	*args;
	char	*redir_string;
	int		to_replace;
	int		builtin;
	struct	s_middle *next;
	struct	s_middle *prev;
}	t_middle;


typedef struct	s_middle_vars
{
	t_middle	*head;
	t_middle	*current;
	bool	in_command;
	char	*command;
	int		to_replace;
}	t_middle_vars;


typedef struct	s_bool_syntax
{
	bool	in_para;
	bool	in_quote;
	bool	in_dquote;
}	t_bool_syntax;

typedef struct s_treenode
{
	int		token; // COMMAND // OR // AND // PIPE
	char	*content;
	t_arg	*args;
	int		to_replace;
	t_redir	*before_redir;
	t_redir	*after_redir;
	int		builtin;
	struct	s_treenode *left;
	struct	s_treenode *right;
} t_treenode;

typedef struct s_data t_data;
// LEXER
t_lex *tokenizer(char *input);
void expand(t_lex *lexed, int mode);
t_lex	*ft_lstnew_lex(char *content, int token, int len);
t_lex	*ft_lstlast_lex(t_lex *lst);
t_lex	*ft_lstfirst_lex(t_lex *lst);
void	ft_lstadd_back_lex(t_lex **lst, t_lex *new);
void ft_lstiter_lex(t_lex *lex);
t_lex *lex_input_checker(t_lex *tokens);
const char* tokenToString(enum e_token t);
int open_checker(t_lex *token);
char **args_to_arr(t_arg *arg);
void handle_space(char *input, int *i, t_lex **head);
void handle_word(char *input, int *i, t_lex **head);
bool is_special(char c);


// MIDDLE MAN
t_middle	*ft_lstnew_middle(char *content, t_arg *args, int token);
void	ft_lstadd_back_middle(t_middle **lst, t_middle *new);
t_middle *make_middle(t_lex *lex);
void ft_lstiter_middle(t_middle *first);
t_middle	*ft_lstlast_middle(t_middle *lst);
t_arg *ft_lstnew_arg(t_lex *word);
t_arg *ft_lstlast_arg(t_arg *head);
void ft_lstaddback_arg(t_arg **head, t_arg *new);
int ft_lstsize_arg(t_arg *arg);

// ABSTRACT SYNTAX TREE
t_treenode		*new_treenode(t_middle *middled);
int counter(t_lex *current, char c);
t_treenode *ruined_tree(t_middle *middled);
t_treenode *parse_cmdline(t_middle **middled);
t_treenode *parse_pipeline(t_middle **middled);
t_redir *ft_lstnew_redir(t_middle *middled);
void ft_lstaddback_redir(t_redir **head, t_redir *new);
void ft_lstiter_redir(t_redir *first);
void print_ascii_tree(t_treenode *root, int level); // FOR DEBUGGING -- SHOULD BE REMOVED LATER

// UTILS
char	*ft_substr(const char *s, unsigned int start, size_t len, int mode);
char	*ft_strdup(const char *s1, int mode);
size_t	ft_strlen(const char *s);
void ft_putstr_fd(int fd, char *str);
char	*ft_strjoin(const char *s1, const char *s2, int mode);
int	ft_strcmp(const char *s1, const char *s2);
char	**ft_split(char const *s, char c, int mode);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char	*ft_strchr(const char *s, int c);
int	ft_atoi(const char *str);
char	*ft_itoa(int n, int mode);
size_t	ft_strcpy(char *dst, const char *src);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int	numlen(int n);
// EXPANDER
char *star_matching(char *to_match);
char *env_expander(char *to_expand, t_env *env);
char *normalize_pattern(char *pattern);
// BUILTINS
int cd(t_treenode *cd_root, t_env **env, t_data *data);
int pwd(t_treenode *pwd_node, t_data *data);
int env(t_env *env);
char *get_pwd(void); // GC does not free this!
int export(t_env **env, t_treenode *export_root);
int unset(t_env **env, t_treenode *unset_root);
int echo(t_treenode *echo_root);
int exit_cmd(t_treenode *root, t_env *env);
int export_core(t_env **env, char *exp_arg);
int exit_core(int status, t_env *env);
// ENV STUFF
t_env *array_to_env(char **env);
void ft_lstiter_env(t_env *env, bool add_declare); // DEBUG
void ft_lstclear_env(t_env *env);
t_env	*ft_lstnew_env(char *env);
void	ft_lstadd_back_env(t_env **lst, t_env *new);
t_env *get_env(t_env *env, char *str);
int ft_lstsize_env(t_env *env);
char **env_to_array(t_env *env);
#endif