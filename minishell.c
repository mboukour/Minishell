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

void get_input() 
{
    char *input;
    t_lex *lexed;
    t_lex *we_check;
    int erorr_flag;

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
		lexed = tokenizer(input);
        we_check = check_valid_input(lexed);
        if(we_check)
        {
            ft_putstr_fd(2, "Parse error near: ");
            ft_putstr_fd(2, we_check->content);
            ft_putstr_fd(2, "\n");
        }
        else        
            ft_lstiter_lex(lexed);
        add_history(input);
        free(input);
    }
}


int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
    if(!isatty(0))
        return 0;
    get_input();
    return 0;
}