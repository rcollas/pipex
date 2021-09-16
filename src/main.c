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

char	**get_binaries_path(char **env)
{
	int	line;

	if (!get_path_line(env))
		return (0);
	line = get_path_line(env);
	return (ft_split(*(env + line), ':'));
}

int		exec(char **path, char *argv, char **env)
{
	int	i;
	char	*cmd;
	char	**cmd_args;

	i = -1;
	(void)env;
	cmd_args = ft_split(argv, ' ');
	while (path[++i])
	{
		path[i] = ft_strjoin(path[i], "/");
		cmd = ft_strjoin(path[i], *cmd_args);
		if (access(cmd, X_OK) == 0) 
		{
			free(cmd);
			return (i);
		}
		free(cmd);
	}
	perror(*cmd_args);
	return (-1);
}

int	child_process(int file1, int *end, t_var *vars)
{
	int	i;
	char	**cmd_args;

	cmd_args = ft_split(vars->argv[2], ' ');
	i = exec(vars->path, vars->argv[2], vars->env);
	if (i == -1)
		return (0);
	dup2(file1, STDIN_FILENO);
	dup2(end[1], STDOUT_FILENO);
	close(end[0]);
	close(file1);
	vars->path[i] = ft_strjoin(vars->path[i], *cmd_args);
	if (execve(vars->path[i], cmd_args, NULL) == -1)
	{
		perror("Execve failed");
		return (0);
	}
	return (1);
}

int	parent_process(int file2, int *end, t_var *vars)
{
	char **cmd_args;
	int	i;

	waitpid(-1, &vars->pid, 0);
	cmd_args = ft_split(vars->argv[3], ' ');
	i = exec(vars->path, vars->argv[3], vars->env);
	if (i == -1)
		return (0);
	dup2(file2, STDOUT_FILENO);
	dup2(end[0], STDIN_FILENO);
	close(end[1]);
	close(file2);
	vars->path[i] = ft_strjoin(vars->path[i], *cmd_args);
	if (execve(vars->path[i], cmd_args, NULL) == -1)
	{
		perror("Execve failed");
		return (0);
	}
	return (1);
}

int	pipex(int file1, int file2, t_var *vars)
{
	int	end[2];

	pipe(end);
	vars->pid = fork();
	if (vars->pid < 0)
	{
		perror("Fork");
		return (0);
	}
	if (vars->pid == 0)
	{
		if (child_process(file1, end, vars) == FAIL)
			return (0);
	}
	else
	{
		if (parent_process(file2, end, vars) == FAIL)
			return (0);
	}
	return (1);
}

int	main(int argc, char **argv, char **env)
{
	int		file1;
	int		file2;
	t_var	vars[0];

	if (argc != 5)
	{
		ft_putstr_fd("Missing arguments\n", 0);
		return (0);
	}
	vars->env = env;
	vars->path = get_binaries_path(env);
	vars->argv = argv;
	file1 = open(argv[1], O_RDONLY);
	file2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC);
	if (file1 < 0 || file2 < 0)
	{
		perror(argv[1]);
		return (-1);
	}
	if (pipex(file1, file2, vars) == FAIL)
		return (0);
}
