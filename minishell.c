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

void display_error(t_lex *lex, t_middle *middle)
{
    char *res;

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

    lexed = tokenizer(input);
    we_check_lex = lex_input_checker(lexed);
    if(we_check_lex)
         display_error(we_check_lex, NULL);
    else
    {
        // ft_lstiter_lex(lexed);
        middled = make_middle(lexed);
        we_check_middled = middle_input_checker(middled);
        if(we_check_middled)
            display_error(NULL, we_check_middled);
        else
            return (ruined_tree(middled));
    }
    return (NULL);
}

void get_input(char **envp) 
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

int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
    if(!isatty(0))
        return 0;
    get_input(env);
    return 0;
}