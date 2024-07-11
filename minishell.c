#include "minishell.h"



void sigint_handler(int sig)
{
    (void)sig;
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void display_error(int error_checker, t_lex *lex, t_env **env)
{
    change_status(env, SYNTAX_ERROR_STATUS);
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


t_treenode *parsing(char *input,  t_env **env)
{
    t_lex *lexed;
    t_lex *we_check_lex;
    t_middle *middled;
    int error_checker;
    int in1;

    lexed = tokenizer(input);
    error_checker = open_checker(lexed);
    if(error_checker != NONE)
        return (display_error(error_checker, NULL, env), NULL);
    quotes_handler(lexed);
    we_check_lex = lex_input_checker(lexed);
    if(we_check_lex)
	{
        in1 = dup(STDIN_FILENO);
		fake_open(lexed, we_check_lex);
        if (dup2(in1, STDIN_FILENO) == -1)
            export_core(env, "?=1");
        infooo = 1337;
        display_error(NONE, we_check_lex, env);
	}
    else
    {
        middled = make_middle(lexed);
        in1 = dup(STDIN_FILENO);
		valid_here_doc(middled);
        if (dup2(in1, STDIN_FILENO) == -1)
        {
            export_core(env, "?=1");
            return (NULL);
        }
        infooo = 1337;
        return (ruined_tree(middled));
    }
    return (NULL);
}

void init_t_data(t_data *data)
{
    data->infile = -1;
    data->outfile = -1;
    data->end[0] = -1;
    data->end[1] = -1;
    data->status = 0;
    data->cmd = NULL;
    data->path = NULL;
    data->env = NULL;
    data->pwd = getcwd(NULL, 0);
    if(!data->pwd)
    {
        ft_putstr_fd(2, "error retrieving current directory: getcwd: cannot access parent directories: ");
        perror("");
    }
    data->old_pwd = NULL;
}

bool is_all_space(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if(!is_ws(str[i]))
            return (false);
        i++;
    }
    return (true);
}


void get_input(t_env **env, t_data *data)
{
    char *input;
    t_treenode *root;

    while (true)
	{
    	signal(SIGQUIT, SIG_IGN);
        signal(SIGINT, sigint_handler);
        input = readline("\x1b[34m🐐 GoatShell\x1b[0m ");
		store_mallocs(input);
        if (!input)
        {
        	ft_putstr_fd(1, "exit\n");
            exit_core(0);
        }
        else if (is_all_space(input))
            continue;
        else if(ft_strcmp(input, ""))
        {
            root = parsing(input, env);
            if(root)
                traverse_tree(root, data, env);
            infooo = 1337;

        }
        add_history(input);
		smart_close();
        smart_free();
    }
}

int no_envp_export(t_env **env, t_data *data)
{
    char *pwd;

    if(data->pwd)
    {
        pwd = ft_strjoin("PWD=", data->pwd, MANUAL);
		null_protector(pwd);
        export_core(env, pwd);
		free(pwd);
    }
    export_core(env, "SHLVL=1");
    export_core(env, SECURE_PATH);
    return (0);
}


int main(int ac, char **av, char **envp)
{
    t_env *env;
    t_data data;

    (void)ac;
    (void)av;
    if(!isatty(0))
        return 0;
    rl_catch_signals = 0;
    init_t_data(&data);
    if(!envp || !*envp)
    {
        env = NULL;
        if(no_envp_export(&env, &data))
            return(1);
    }
    else
    {
        env = array_to_env(envp);
        if(!env)
        {
            ft_putstr_fd(2, FAILURE_MSG);
            return (1);
        }
    }
    export_core(&env, "?=0");
    get_input(&env, &data);
    return (0);
}