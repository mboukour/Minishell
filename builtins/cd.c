#include "../minishell.h"

void safe_free(char **ptr)
{
    if (ptr && *ptr)
    {
        free(*ptr);
        *ptr = NULL;
    }
}

char *remove_last_slash(char *str)
{
    int i;
    int last_slash_index;
    char *result;

    i = 0;
    last_slash_index = -1;
    while (str[i])
    {
        if (str[i] == '/')
            last_slash_index = i;
        i++;
    }
    if (last_slash_index == -1)
        return str;
    result = malloc(last_slash_index + 1);
    if(!result)
		return (free(str), null_protector(result), NULL);
    ft_strncpy(result, str, last_slash_index + 1);
    result[last_slash_index] = '\0';
    free(str);
    return result;
}

void cd_error()
{
    ft_putstr_fd(2, "cd: error retrieving current directory: ");
    ft_putstr_fd(2, "getcwd: cannot access parent directories: ");
    perror("");
}

int check_removed(char *path, t_data *data, t_env **env)
{
    char *dir;
    char *new;
    char *result;
    int r;
	char *temp;

    dir = getcwd(NULL, 0);
    r = 0;
    if (!dir)
    {
        new = ft_strjoin(data->pwd, "/", MANUAL);
		null_protector(new);
		temp = new;
		new = ft_strjoin(new, path, MANUAL);
		free(temp);
		null_protector(new);
        safe_free(&data->old_pwd);
        data->old_pwd = data->pwd;
        data->pwd = new;
        r = 1;
        if (!ft_strcmp("..", path))
        {
            *(data->foolproof_wd) = remove_last_slash(*(data->foolproof_wd));
            if (!chdir(*(data->foolproof_wd)))
            {
                data->pwd = remove_last_slash(data->pwd);
                safe_free(&data->old_pwd);
                data->old_pwd = data->pwd;
                data->pwd = ft_strdup(*(data->foolproof_wd), MANUAL);
				store_malloced(data->pwd);
                safe_free(data->foolproof_wd);
            }
            else
                cd_error();
            r = 2;
        }
        else if (!ft_strcmp(".", path))
        {
            cd_error();
            r = 2;
        }
        else
            cd_error();
        result = ft_strjoin("PWD=", data->pwd, MANUAL);
		null_protector(result);
		export_core(env, result);
		free(result);
        result = ft_strjoin("OLDPWD=", data->old_pwd, MANUAL);
		null_protector(result);
		export_core(env, result);
		free(result);
    }
    safe_free(&dir);
    return r;
}

int cd_core(char *path, t_env **env, t_data *data)
{
    char *wd;
    char *result;
    int check;

    check = check_removed(path, data, env);
    if (check == 1)
        return 1;
    else if (check == 2)
        return 0;

    if (chdir(path))
    {
        ft_putstr_fd(2, "cd: ");
        ft_putstr_fd(2, path);
        ft_putstr_fd(2, " no such file or directory\n");
        return 1;
    }
    wd = getcwd(NULL, 0);
    if (!wd)
    {
        ft_putstr_fd(2, "cd: can't get current working directory\n");
        return 1;
    }
    safe_free(&data->old_pwd);
    data->old_pwd = ft_strdup(data->pwd, MANUAL);
	store_malloced(data->old_pwd);
    safe_free(&data->pwd);
    data->pwd = wd;
	store_malloced(data->pwd);
    if (ft_strcmp("..", data->pwd))
    {
        safe_free(data->foolproof_wd);
        *(data->foolproof_wd) = ft_strdup(data->pwd, MANUAL);
		store_malloced(*(data->foolproof_wd));
    }
    result = ft_strjoin("OLDPWD=", data->old_pwd, MANUAL);
	null_protector(result);
	export_core(env, result);
	free(result);
    result = ft_strjoin("PWD=", data->pwd, MANUAL);
	null_protector(result);
	export_core(env, result);
	free(result);
    return 0;
}

int cd_home(t_env **env, t_data *data)
{
    t_env *home_env;
    char *home;

    home_env = get_env(*env, "HOME");
    if (!home_env)
    {
        ft_putstr_fd(2, "cd: HOME not set\n");
        return 1;
    }
    return cd_core(home_env->value + 5, env, data);
}

int cd(t_treenode *cd_root, t_env **env, t_data *data)
{
    t_arg *args;
    static char *foolproof_wd;

    args = cd_root->args;
    data->foolproof_wd = &foolproof_wd;
    if (!args)
        return cd_home(env, data);
    else if (!args->next)
        return cd_core(args->content, env, data);
    ft_putstr_fd(2, "cd: too many arguments\n");
    return 1;
}
