#ifndef PIPEX_H
# define PIPEX_H

#include "../libft/libft.h"
#include <unistd.h>
#include  <stdio.h>
#include <fcntl.h>

typedef struct s_var {
	char	**argv;
	char	**env;
	char	**path;
	int		*end;
	char	*cmd1;
	char	*cmd2;
} 			t_var;

#endif
