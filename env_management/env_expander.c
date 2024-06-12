#include "../minishell.h"


int get_current_path(char *str)
{
	int i;
	
	i = 0;
	while(str[i] && str[i] != '$' && str[i] != '*')
		i++;
	return i;
}

t_lex	*ft_mini_lstnew_lex(char *content, int token, int len)
{
	t_lex	*new_node;

	new_node = smart_malloc(sizeof(t_lex));
	new_node->content = content;
	new_node->len = len;
	new_node->token = token;
	new_node->to_replace = REPLACE_ALL;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}



void mini_handle_word(char *input, int *i, t_lex **head)
{
	t_lex *current_node;
    char *content;
    int hold;

	hold = *i;
    while (input[*i] && !is_special(input[*i]) && input[*i] != ' ')
        (*i)++;
    content = ft_substr(input, hold, *i - hold, GC);
    current_node = ft_mini_lstnew_lex(content, WORD, *i - hold);
    ft_lstadd_back_lex(head, current_node);
}


void mini_handle_space(char *input, int *i, t_lex **head)
{
    int hold;
    char *content;
    t_lex *current_node;

	hold = *i;
    while (input[*i] == ' ')
        (*i)++;
    content = ft_substr(input, hold, *i - hold, GC);
    current_node = ft_mini_lstnew_lex(content, WHITE_SPACE, *i - hold);
    ft_lstadd_back_lex(head, current_node);
}


void mini_handle_star(char *input, int *i, t_lex **head)
{
	t_lex *current_node;
    int hold;
    int c;
    char *content;

    c = input[*i];
	hold = *i;
    (*i)++;
    while (input[*i] && (input[*i] == '*'|| !is_special(input[*i])) && input[*i] != ' ')
        (*i)++;
    content = ft_substr(input, hold, *i - hold, GC);
    current_node = ft_mini_lstnew_lex(content, c, *i - hold);
    ft_lstadd_back_lex(head, current_node);
}

void mini_handle_env(char *input, int *i, t_lex **head)
{
	t_lex *current_node;
    int hold;
    int c;
    char *content;
    bool star_bool;

    star_bool = false;
    c = input[*i];
	hold = *i;
    (*i)++;
    while (input[*i] && (input[*i] == '$' || !is_special(input[*i])) && input[*i] != ' ')
        (*i)++;
    content = ft_substr(input, hold, *i - hold, GC);
    current_node = ft_mini_lstnew_lex(content, c, *i - hold);
    ft_lstadd_back_lex(head, current_node);
}


t_lex *mini_lexer(char *str)
{
	int i;
	t_lex *head;
	t_lex *current;

	i = 0;
	head = NULL;
	while(str[i])
	{
		if(str[i] == ' ')
			mini_handle_space(str, &i, &head);
		else if (str[i] == '*')
			mini_handle_star(str, &i, &head);
		else if (str[i] == '$')
			mini_handle_env(str, &i, &head);
		else
			mini_handle_word(str, &i, &head);
	}
	return (head);
}

char *env_expander(char *to_expand, t_env *env)
{
	t_lex *mini_lexed;
	t_lex *original;
	t_env *env_node;
	char *me;

	mini_lexed = mini_lexer(to_expand);
	original = mini_lexed;
	while(mini_lexed)
	{ 
		if(mini_lexed->token == ENV)
		{
			env_node = get_env(env, mini_lexed->content + 1);
			if(!env_node)
				mini_lexed->content = ft_strdup("", GC);
			else
			{
				me = env_node->value;
				while(*me && *me != '=')
					me++;
				me++;
				mini_lexed->content = ft_strdup(me, GC);
			}
		}
		mini_lexed = mini_lexed->next;
	}
	me = NULL;
	while(original)
	{
		me = ft_strjoin(me, original->content, GC);
		original = original->next;
	}
	return (me);
}