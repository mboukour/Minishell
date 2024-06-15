#include "../minishell.h"

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
        {
            last_slash_index = i;
        }
        i++;
    }
    if (last_slash_index == -1)
        return str;
    result = malloc(last_slash_index + 1);
    if (!result)
        return NULL;
    strncpy(result, str, last_slash_index);
    result[last_slash_index] = '\0';
    free(str);
    return result;
}

int check_removed(char *path, t_data *data, t_env **env)
{
	char *dir;
	char *new;
	char *result;

	dir = getcwd(NULL, 0);
	if(!dir)
	{
		new = ft_strjoin(data->pwd, "/", MANUAL);
		new = ft_strjoin(new, path, MANUAL);
		data->old_pwd = data->pwd;
		result = ft_strjoin("OLDPWD=", data->old_pwd, MANUAL);
		export_core(env, result);
		free(result);
		data->pwd = new;
		*(data->foolproof_wd) = remove_last_slash(*(data->foolproof_wd));
		if(!chdir(*(data->foolproof_wd)))
		{
			data->pwd = remove_last_slash(data->pwd);
			data->old_pwd = data->pwd;
			result = ft_strjoin("OLDPWD=", data->old_pwd, MANUAL);
			export_core(env, result);
			free(result);
			data->pwd = ft_strdup(*(data->foolproof_wd), MANUAL);
			free(*(data->foolproof_wd));
			*(data->foolproof_wd) = NULL;
		}
		return (1);
	}
	free(dir);
	return (0);
}

int cd_core(char *path, t_env **env, t_data *data)
{
	char *wd;
	char *result;
	int check;

	check = check_removed(path, data, env);
	if(check)
		return (1);
	wd = getcwd(NULL, 0);
	if(!wd)
	{
		ft_putstr_fd(2, "cd: can't get current working directory\n");
		return (1);
	}
	result = ft_strjoin("OLDPWD=", wd, MANUAL);
	free(wd);
	if(!result)
	{
		ft_putstr_fd(2, FAILURE_MSG);
		return (1);
	}
	if(chdir(path))
	{
		ft_putstr_fd(2, "cd: ");
		ft_putstr_fd(2, path);
		ft_putstr_fd(2, " no such file or directory\n");
		return (1);
	}
	wd = getcwd(NULL, 0);
	if(!wd)
	{
		ft_putstr_fd(2, "cd: can't get current working directory\n");
		return (1);
	}
	free(data->pwd);
	free(data->old_pwd);
	data->pwd = wd; // SHOULD BE FREED
	data->old_pwd = result; // SHOULD BE FREED
	if(ft_strcmp("..", data->pwd))
		*(data->foolproof_wd) = ft_strdup(data->pwd, MANUAL);
	export_core(env, result);
	free(result);
	result = ft_strjoin("PWD=", wd, MANUAL);
	export_core(env, result);
	free(result);
	return (0);
}

int cd_home(t_env **env, t_data *data)
{
	t_env *home_env;
	char *home;

	home_env = get_env(*env, "HOME");
	if(!home_env)
	{
		ft_putstr_fd(2, "cd: HOME not set\n");
		return (1);
	}
	return cd_core(home_env->value + 5, env, data);
}

int cd(t_treenode *cd_root, t_env **env, t_data *data)
{
	t_arg *args;
	static char *foolproof_wd;

	args = cd_root->args;
	data->foolproof_wd = &foolproof_wd;
	if(!args)
		return (cd_home(env, data));
	else if (!args->next)
		return (cd_core(args->content, env, data));
	ft_putstr_fd(2, "cd: too many arguments\n");
	return (1);
}