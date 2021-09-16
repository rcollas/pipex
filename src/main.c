#include "../include/pipex.h"

int	get_path_line(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
			return (i);
		i++;
	}
	return (0);
}

char	**get_binaries_path(char **env, char **paths)
{
	int	line;

	if (!get_path_line(env))
		return (0);
	line = get_path_line(env);
	paths = ft_split(*(env + line), ':');
	return (ft_split(*(env + line), ':'));
}

int	main(int argc, char **argv, char **env)
{
	char	**paths;
	char	**cmd_args;
	char	*cmd;
	int	i;

	i = -1;
	(void)argc;
	paths = NULL;
	cmd_args = NULL;
	cmd_args = ft_split(argv[2], ' ');
	paths = get_binaries_path(env, paths);
	while (paths[++i])
	{
		paths[i] = ft_strjoin(paths[i], "/");
		cmd = ft_strjoin(paths[i], argv[2]);
		printf("cmd is %s \n", cmd);
		if (execve(cmd, cmd_args, env) > 0) 
		{
			perror("Error");
			break ;
		}
		free(cmd);
	}
}
