#include "../minishell.h"
// pipe(fd);
// dup2(fd[1], 1);
// executioner(root->left, getenv("PATH"), envp, fd[0]);
// close(fd[1]);
// dup2(save_in, 0);
// dup2(save_out, 1);
// dup2(fd[0], 0);
// executioner(root->right, getenv("PATH"), envp, fd[1]);
// close(fd[0]);
// close(fd[1]);
// dup2(save_in, 0);
// dup2(save_out, 1);
