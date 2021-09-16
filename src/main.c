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
		printf("cmd is %s\n", cmd);
		printf("i = %d\n", i);
		if (access(cmd, X_OK) == 0) 
		{
			free(cmd);
			return (i);
		}
		free(cmd);
	}
	return (0);
}

void	child_process(int file1, char *cmd1, int *end, t_var *vars)
{
	int	i;
	char	**cmd_args;

	(void)cmd1;
	vars->path = get_binaries_path(vars->env);
	cmd_args = ft_split(vars->argv[2], ' ');
	i = exec(vars->path, vars->argv[2], vars->env);
	exec(vars->path, vars->argv[2], vars->env);
	dup2(file1, STDIN_FILENO);
	dup2(end[1], STDOUT_FILENO);
	close(end[0]);
	close(file1);
	execve(vars->path[i], cmd_args, vars->env);
}

void	parent_process(int file2, char *cmd2, int *end, t_var *vars)
{
	char **cmd_args;
	int	status;
	int	i;

	(void)cmd2;
	cmd_args = ft_split(vars->argv[3], ' ');
	i = exec(vars->path, vars->argv[3], vars->env);
	wait(&status);
	dup2(file2, STDOUT_FILENO);
	dup2(end[0], STDIN_FILENO);
	close(end[1]);
	close(file2);
	execve(vars->path[i], cmd_args, vars->env);
}

void	pipex(int file1, int file2, t_var *vars)
{
	int	end[2];
	pid_t	parent;

	(void)file1;
	(void)file2;
	(void)vars;
	pipe(end);
	parent = fork();
	if (parent < 0)
		return (perror("Fork: "));
	if (parent == 0)
		child_process(file1, vars->cmd1, end, vars);
	else
		parent_process(file2, vars->cmd2, end, vars);
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
	(void)argv;
	vars->env = env;
	vars->path = get_binaries_path(env);
	vars->argv = argv;
	file1 = open(argv[1], O_RDONLY);
	file2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (file1 < 0 || file2 < 0)
	{
		perror(argv[1]);
		return (-1);
	}
	pipex(file1, file2, vars);
}
