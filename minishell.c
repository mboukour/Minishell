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

void display_error(int error_checker, t_lex *lex, t_middle *middle)
{
    char *res;

    if(error_checker)
    {
        if(error_checker == QUOTE)
            ft_putstr_fd(2, QUOTE_ERROR);
        else
            ft_putstr_fd(2, PARA_ERROR);
        return;
    }
    res = "UNKOWN";
    if(lex)
        res = lex->content;
    else if(middle)
        res = middle->content;
    ft_putstr_fd(2, PARSE_ERROR);
    ft_putstr_fd(2, res);
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
    error_checker = false;
    we_check_lex = lex_input_checker(lexed, &error_checker);
    if(we_check_lex || error_checker)
         display_error(error_checker, we_check_lex, NULL);
    else
    {
        expand(lexed);
        middled = make_middle(lexed);
        we_check_middled = middle_input_checker(middled);
        if(we_check_middled)
            display_error(false, NULL, we_check_middled);
        else
            return (ruined_tree(middled));
    }
    return (NULL);
}

void get_input(t_env **env) 
{
    char *input;
    t_treenode *root;

    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);
    while (true)
	{
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
                print_ascii_tree(root, 0); // EXEUCTION SHOULD GO HERE!!!
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
    env = array_to_env(envp);
    if(!env)
    {
        ft_putstr_fd(2, FAILURE_MSG);
        return (1);
    }
    get_input(&env);
    ft_lstclear_env(env);
    return 0;
}