#include "../minishell.h"

int check_removed(char *path, t_data *data)
{
	char *dir;
	char *new;

	dir = getcwd(NULL, 0);
	if(!dir)
	{
		fprintf(stderr, "Removed\n");
		new = ft_strjoin(data->pwd, "/", MANUAL);
		fprintf(stderr, "%s\n", new);
		new = ft_strjoin(new, path, MANUAL);
		data->old_pwd = data->pwd;
		data->pwd = new;
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

	check = check_removed(path, data);
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
	data->pwd = wd; // SHOULD BE FREED\
	data->old_pwd = result; // SHOULD BE FREED
	export_core(env, result);
	free(result);
	result = ft_strjoin("PWD=", wd, MANUAL);
	export_core(env, result);
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

	args = cd_root->args;
	args = args->next;
	if(!args)
		return (cd_home(env, data));
	else if (!args->next)
		return (cd_core(args->content, env, data));
	ft_putstr_fd(2, "cd: too many arguments\n");
	return (1);
}