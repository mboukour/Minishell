#include "../minishell.h"


char *access_real_path(const char *path, const char *loop_for)
{
    struct dirent *entry;
    DIR *dir;
	char *result;

	// fprintf(stderr, "%s\n", path);
	dir = opendir(path);
    if (dir == NULL)
    {
        perror("Unable to open directory");
		error_procedure();
    }
    while ((entry = readdir(dir)))
    {
        if (!ft_strcasecmp(entry->d_name, loop_for))
        {
			result = ft_strjoin(path, "/", GC);
			result = ft_strjoin(result ,entry->d_name, GC);
            closedir(dir);
            return (result);
        }
    }
    closedir(dir);
    return (NULL);
}