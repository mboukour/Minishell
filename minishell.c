#include "minishell.h"

void sigint_handler(int sig)
{
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void sigquit_handler(int sig)
{
    rl_on_new_line();
    rl_redisplay();
}

void display_error(int error_checker, t_lex *lex)
{
    if(error_checker != NONE)
    {
        if(error_checker == QUOTE)
            ft_putstr_fd(2, QUOTE_ERROR);
        else
            ft_putstr_fd(2, PARA_ERROR);
        return;
    }
    ft_putstr_fd(2, PARSE_ERROR);
    ft_putstr_fd(2, lex->content);
    ft_putstr_fd(2, "\n");
}

t_treenode *parsing(char *input)
{
    t_lex *lexed;
    t_lex *we_check_lex;
    t_middle *we_check_middled;
    t_middle *middled;
    int error_checker;

    lexed = tokenizer(input);
    error_checker = open_checker(lexed);
    if(error_checker != NONE)
        return (display_error(error_checker, NULL), NULL);
    expand(lexed, QUOTE);
    we_check_lex = lex_input_checker(lexed);
    if(we_check_lex)
         display_error(NONE, we_check_lex);
    else
    {
        middled = make_middle(lexed);
        return (ruined_tree(middled));
    }
    return (NULL);
}

void init_t_data(t_data *data)
{
    if (data == NULL)
        return;
    data->infile = -1;
    data->outfile = -1;
    data->end[0] = -1;
    data->end[1] = -1;
    data->status = 0;
    data->cmd = NULL;
    data->path = NULL;
    data->env = NULL;
}


void get_input(t_env **env) 
{
    char *input;
    t_treenode *root;
    t_data data;

    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);
    while (true)
	{
        init_t_data(&data);
        input = readline("\x1b[34m🐐 GoatShell\x1b[0m ");
        if (!input)
        {
            rl_clear_history();
            break;
        }
        else if(ft_strcmp(input, ""))
        {
            root = parsing(input);
            if(root)
			{
                traverse_tree(root, &data, env);
                // print_ascii_tree(root, 0); // EXEUCTION SHOULD GO HERE!!!
			}
        }
        add_history(input);
        free(input);
        smart_free();
    }
}

int main(int ac, char **av, char **envp)
{
    t_env *env;

    (void)ac;
    (void)av;
    if(!isatty(0))
        return 0;
    rl_catch_signals = 0; 
    env = array_to_env(envp);
    if(!env)
    {
        ft_putstr_fd(2, FAILURE_MSG);
        return (1);
    }
    export_core(&env, "?=0");
    get_input(&env);
    ft_lstclear_env(env);
    return 0;
}