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

void get_input(char **envp) 
{
    char *input;
    t_lex *lexed;
    t_lex *we_check_lex;
    t_middle *we_check_middled;
    t_middle *middled;
    int erorr_flag;
    t_treenode *root;

    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);
    root = NULL;
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
            lexed = tokenizer(input);
            we_check_lex = lex_input_checker(lexed);
            if(we_check_lex)
            {
                ft_putstr_fd(2, "Parse error near: ");
                ft_putstr_fd(2, we_check_lex->content);
                ft_putstr_fd(2, "\n");
            }
            else
            {
                middled = make_middle(lexed);
                we_check_middled = middle_input_checker(middled);
                if(we_check_middled)
                {
                    ft_putstr_fd(2, "Parse error near: ");
                    ft_putstr_fd(2, we_check_middled->content);
                    ft_putstr_fd(2, "\n");
                }
                else
                    root = ruined_tree(middled); // Can tree fail too?
            }
            // executioner(root, getenv("PATH"), envp);
            // ft_lstiter_middle(middled);
            //     ft_lstiter_lex(lexed);
            // }
            add_history(input);
        }
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