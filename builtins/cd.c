#include "../minishell.h"


int cd_core(char *path, t_env **env)
{
	char *wd;
	char *result;

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
	export_core(env, result);
	free(result);
	if(chdir(path))
	{
		fprintf(stderr, "PATH: %s ", path);
		ft_putstr_fd(2, "CD Error\n");
		return (1);
	}
	wd = getcwd(NULL, 0);
	if(!wd)
	{
		ft_putstr_fd(2, "cd: can't get current working directory\n");
		return (1);
	}
	result = ft_strjoin("PWD=", wd, MANUAL);
	export_core(env, result);
	free(wd);
	return (0);
}

int cd_home(t_env **env)
{
	t_env *home_env;
	char *home;

	home_env = get_env(*env, "HOME");
	if(!home_env)
	{
		ft_putstr_fd(2, "cd: HOME not set\n");
		return (1);
	}
	return cd_core(home_env->value + 5, env);
}


int cd(t_treenode *cd_root, t_env **env)
{
	t_arg *args;

	args = cd_root->args;
	args = args->next;
	if(!args)
		return (cd_home(env));
	else if (!args->next)
		return (cd_core(args->content, env));
	ft_putstr_fd(2, "cd: too many arguments\n");
	return (1);
}