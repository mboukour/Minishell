#include "../minishell.h"

static void	slash_paths(char **binary_paths)
{
	int	i;

	i = 0;
	while (binary_paths[i])
	{
		binary_paths[i] = ft_strjoin(binary_paths[i], "/", GC);
		i++;
	}
}

char	**get_paths(char **envp, char *path)
{
	char	**binary_paths;

	binary_paths = ft_split(path, ':', GC);
	slash_paths(binary_paths);
	return (binary_paths);
}
int executioner(t_treenode *command, char *path, char **envp, int fd)
{
	int i;
	int pid;
	char **bin_paths;
	char *cmd_path;

	i = 0;
	pid = fork();
	if(!pid)
	{
		close(fd);
		// fprintf(stderr, "Starting: %s\n", command->content);
		execve(command->content, command->args, envp);
		bin_paths = get_paths(envp, path);
		while(bin_paths[i])
		{
			cmd_path = ft_strjoin(bin_paths[i], command->content, GC);
			execve(cmd_path, command->args, envp);
			i++;
		}
		perror("Error: ");
		exit(EXIT_FAILURE);
	}
	return (pid);
}
