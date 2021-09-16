#ifndef PIPEX_H
# define PIPEX_H
# define FAIL 0
# define SUCCESS 1

#include "../libft/libft.h"
#include <unistd.h>
#include  <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef struct s_var {
	char	**argv;
	char	**env;
	char	**path;
	int		*end;
	pid_t	pid;
} 			t_var;

#endif
