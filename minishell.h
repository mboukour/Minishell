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
#include "./garbage_collector/cgc.h"

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

typedef struct s_env
{
	int index;
	char *name;
	char *value;
	struct s_env *next;
	struct s_env *prev;
} t_env;

typedef struct s_lex
{
	char *content;
	int len;
	bool to_replace;
	enum e_token token;
	enum e_state state;
	struct s_lex *prev;
	struct s_lex *next;
} t_lex;

typedef struct s_redir
{
	int token;
	bool to_replace;
	char *redir_string;
	struct s_redir *next;
} t_redir;

typedef struct s_arg
{
	char *content;
	bool to_replace;
	struct s_arg *next;
} t_arg;

typedef struct s_middle
{
	int token;
	char *content;
	t_arg *args;
	char *redir_string;
	bool to_replace;
	struct s_middle *next;
	struct s_middle *prev;
} t_middle;


typedef struct s_middle_vars
{
	t_middle *head;
	t_middle *current;
	bool in_command;
	char *command;
	bool to_replace;
} t_middle_vars;


typedef struct s_bool_syntax
{
	bool in_para;
	bool in_quote;
	bool in_dquote;
} t_bool_syntax;

typedef struct s_treenode
{
	int token; // COMMAND // OR // AND // PIPE
	char *content;
	t_arg *args;
	bool is_before_redirected;
	bool is_after_redirected;
	bool to_replace;
	t_redir *before_redir;
	t_redir *after_redir;
	struct s_treenode *left;
	struct s_treenode *right;
} t_treenode;


// LEXER
t_lex *tokenizer(char *input);
void expand(t_lex *lexed, int mode);
t_lex	*ft_lstnew_lex(char *content, int token, int len);
t_lex	*ft_lstlast_lex(t_lex *lst);
t_lex	*ft_lstfirst_lex(t_lex *lst);
void	ft_lstadd_back_lex(t_lex **lst, t_lex *new);
void ft_lstiter_lex(t_lex *lex);
t_lex *lex_input_checker(t_lex *tokens, int *error_checker);
const char* tokenToString(enum e_token t);
bool quote_checker(t_lex *token);

// MIDDLE MAN
t_middle	*ft_lstnew_middle(char *content, t_arg *args, int token);
void	ft_lstadd_back_middle(t_middle **lst, t_middle *new);
t_middle *make_middle(t_lex *lex);
void ft_lstiter_middle(t_middle *first);
t_middle	*ft_lstlast_middle(t_middle *lst);
t_arg *ft_lstnew_arg(t_lex *word);
t_arg *ft_lstlast_arg(t_arg *head);
void ft_lstaddback_arg(t_arg **head, t_arg *new);

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
int	ft_strncmp(const char *s1, const char *s2, size_t n);

// EXECUTIONER


// BUILTINS

char *get_pwd(void); // GC does not free this!
int export(t_env **env, char *exp_arg);
int unset(t_env **env, char *to_unset);

// ENV STUFF
t_env *array_to_env(char **env);
void ft_lstiter_env(t_env *env); // DEBUG
void ft_lstclear_env(t_env *env);
t_env	*ft_lstnew_env(char *env);
void	ft_lstadd_back_env(t_env **lst, t_env *new);
t_env *get_env(t_env *env, char *str);
int ft_lstsize_env(t_env *env);

#endif