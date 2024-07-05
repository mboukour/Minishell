#include "../minishell.h"


bool is_ws(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}
