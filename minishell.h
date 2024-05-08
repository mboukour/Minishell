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


# define NONE -1

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

typedef struct s_lex
{
	char *content;
	int len;
	enum e_token token;
	enum e_state state;
	struct s_lex *prev;
	struct s_lex *next;
} t_lex;


typedef struct s_middle
{
	char *content;
	char **args;
	int condition_count;
	int token;
	bool is_in_para;
	struct s_middle *next;
	struct s_middle *prev;
} t_middle;


typedef struct s_treenode
{
	// int token;
	// char *content;
	// char **args;
	t_middle *first;
	t_middle *last;
	bool is_terminal;
	int condition_count;
	struct s_treenode *left;
	struct s_treenode *right;
} t_treenode;


// LEXER
t_lex *tokenizer(char *input);
char *expand(t_lex *lex); // MALLOCS and can fail
t_lex	*ft_lstnew_lex(char *content, int token, int len);
t_lex	*ft_lstlast_lex(t_lex *lst);
t_lex	*ft_lstfirst_lex(t_lex *lst);
void	ft_lstadd_back_lex(t_lex **lst, t_lex *new);
void ft_lstiter_lex(t_lex *lex);
t_lex *check_valid_input(t_lex *tokens);
const char* tokenToString(enum e_token t);
// MIDDLE MAN
t_middle	*ft_lstnew_middle(char *content, char **args, int token);
void	ft_lstadd_back_middle(t_middle **lst, t_middle *new);
t_middle *make_middle(t_lex *lex);
void ft_lstiter_middle(t_middle *first);


// ABSTRACT SYNTAX TREE
t_treenode		*ft_lstnew_treenode(t_middle *first, t_middle *last, bool is_terminal);
t_treenode *rdp(t_middle *middle, t_treenode *current_root);
void change_root_to(t_treenode **from_node, t_treenode *to_node);
int counter(t_lex *current, char c);

// UTILS
char	*ft_substr(const char *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s1);
size_t	ft_strlen(const char *s);
void ft_putstr_fd(int fd, char *str);
char	*ft_strjoin(const char *s1, const char *s2);


// BUILTINS
char *get_pwd(void); // MALLOCS, NEEDS FREE !!!!endif


#endif