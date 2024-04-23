#include "minishell.h"

void get_input() 
{
    char *input;

    while (true) 
	{
        input = readline("\x1b[34m🐐 GoatShell\x1b[0m ");
        if (!input) 
		{
            printf("\n");
            break;
        }
		lexer(input);
        add_history(input);
        free(input);
    }
}

int main()
{
    get_input();
    return 0;
}