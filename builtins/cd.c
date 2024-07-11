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
    result = smart_malloc(last_slash_index + 1);
    if(!result)
		return (free(str), null_protector(result), NULL);
    ft_strncpy(result, str, last_slash_index + 1);
    result[last_slash_index] = '\0';
    // free(str);
    return result;
}

void cd_error()
{
    ft_putstr_fd(2, "cd: error retrieving current directory: ");
    ft_putstr_fd(2, "getcwd: cannot access parent directories: ");
    perror("");
}

void remove_additional_slashes(char *path)
{
    int i;

    i = 0;
    while(path[i])
        i++;
    i--;
    while(path[i] == '/')
        path[i--] = '\0';
}

char *get_new_path(char *pwd, char *original)
{
    int i;
    int j = 0;
    char **sp_res;
    char *path;

    i = 0;

    sp_res = ft_split(pwd, '/', GC);
    while(sp_res[i] && ft_strcmp(sp_res[i], "..") && ft_strcmp(sp_res[i], "."))
        i++;
    path = "/";
    while(j < i)
    {
        path = ft_strjoin(path, sp_res[j], GC);
        path = ft_strjoin(path, "/", GC);
        j++;
    }
    remove_additional_slashes(path);
    while(sp_res[i])
        i++;
    i--;
    while(sp_res[i] && (!ft_strcmp(sp_res[i], "..") || !ft_strcmp(sp_res[i], ".")))
    {
        if(ft_strlen(sp_res[i]) == 2)
        {
            path = remove_last_slash(path);
        }
        i--;
    }
    i = 0;
    sp_res = ft_split(original, '/', GC);
    if(!sp_res)
        return (NULL);
    while(sp_res[i])
    {
        if(!ft_strcmp(sp_res[i], ".."))
            path = remove_last_slash(path);
        else
        {
            path = ft_strjoin(path, "/", GC);
            path = ft_strjoin(path, sp_res[i], GC);
        }
        i++;
    }
    return (path);
}

#include <stdbool.h>

bool is_all_points(char *path)
{
    int i = 0;
    int dot_count = 0;

    while (path[i])
    {
        if (path[i] != '/')
        {
            if (path[i] == '.')
                dot_count++;
            else
                dot_count = 0;
            if (dot_count > 2)
                return false;
        }
        else
        {
            if (dot_count > 2)
                return false;
            dot_count = 0;
        }
        i++;
    }
    if (dot_count > 2)
        return false;

    return true;
}


char *add_slash_if_needed(char *path)
{
    int i;

    i = 0;
    while(path[i])
        i++;
    if(path[i] != '/')
    {
        remove_ptr(path);
        path = ft_strjoin(path, "/", GC);
    }
    else
        path = ft_strdup(path, GC);
    return (path);
}

int check_removed(char *path, t_data *data, t_env **env)
{
    char *dir;
    char *new;
    char *result;
    int r;

    dir = getcwd(NULL, 0);
    r = 0;
    if (!dir)
    {
        new = get_new_path(data->pwd, path);
        if(!chdir(new))
        {
            safe_free(&data->old_pwd);
            data->old_pwd = ft_strdup(data->pwd, MANUAL);
            null_protector(data->old_pwd);
            safe_free(&data->pwd);
            data->pwd = ft_strdup(new, MANUAL);
            r = 2;
        }
        else
        {
            if(is_all_points(path))
            {
                safe_free(&data->old_pwd);
                data->old_pwd = ft_strdup(data->pwd, MANUAL);
                null_protector(data->old_pwd);
                new = add_slash_if_needed(data->pwd);
                safe_free(&data->pwd);
                data->pwd = ft_strjoin(new, path, MANUAL);
                null_protector(data->pwd);
                cd_error();
            }
            else
                ft_putstr_fd(2, "cd: No such file or directory\n");
            r = 1;
        }
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
    data->pwd = ft_strdup(wd, MANUAL);
	free(wd);
	store_malloced(data->pwd);
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

    args = cd_root->args;
    if (!args)
        return cd_home(env, data);
    else if (!args->next)
        return cd_core(args->content, env, data);
    ft_putstr_fd(2, "cd: too many arguments\n");
    return 1;
}
