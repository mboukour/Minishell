#include "../minishell.h"


void save_terminal_settings(struct termios *orig_termios)
{
    if (tcgetattr(STDIN_FILENO, orig_termios) != 0) 
	{
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }
}


void restore_terminal_settings(struct termios *orig_termios) 
{
    if (tcsetattr(STDIN_FILENO, TCSANOW, orig_termios) != 0) 
	{
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}