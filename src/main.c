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

int	free_arg(char **cmd_args)
{
	int	i;

	i = -1;
	while (cmd_args[++i])
		free(cmd_args[i]);
	free(cmd_args);
	return (0);
}

int	exec(char **path, char *argv)
{
	int		i;
	char	*cmd;
	char	**cmd_args;
	char	**to_free;
	char	*old_path;

	i = -1;
	cmd_args = ft_split(argv, ' ');
	to_free = path;
	while (path[++i])
	{
		old_path = path[i];
		path[i] = ft_strjoin(path[i], "/");
		free(old_path);
		cmd = ft_strjoin(path[i], *cmd_args);
		if (access(cmd, X_OK) == 0)
		{
			free(cmd);
			free_arg(cmd_args);
			return (i);
		}
		free(cmd);
	}
	perror(*cmd_args);
	return (free_arg(cmd_args) + free_arg(to_free));
}

int	first_child(int file1, int *end, t_var *vars)
{
	int		i;
	char	**cmd_args;

	cmd_args = ft_split(vars->argv[2], ' ');
	i = exec(vars->path, vars->argv[2]);
	if (i == -1)
	{
		return (free_arg(cmd_args));
	}
	dup2(file1, STDIN_FILENO);
	dup2(end[1], STDOUT_FILENO);
	close(end[0]);
	close(file1);
	vars->path[i] = ft_strjoin(vars->path[i], *cmd_args);
	if (execve(vars->path[i], cmd_args, NULL) == FAIL)
	{
		perror("Execve failed");
		return (free_arg(cmd_args));
	}
	return (free_arg(cmd_args));
}

int	second_child(int file2, int *end, t_var *vars)
{
	int		i;
	char	**cmd_args;

	cmd_args = ft_split(vars->argv[3], ' ');
	i = exec(vars->path, vars->argv[3]);
	if (i == -1)
		return (free_arg(cmd_args));
	dup2(file2, STDOUT_FILENO);
	dup2(end[0], STDIN_FILENO);
	close(end[1]);
	close(file2);
	vars->path[i] = ft_strjoin(vars->path[i], *cmd_args);
	if (execve(vars->path[i], cmd_args, NULL) == -1)
	{
		perror("Execve failed");
		return (free_arg(cmd_args));
	}
	return (free_arg(cmd_args));
}

int	pipex(int file1, int file2, t_var *vars)
{
	int	end[2];
	int	status;

	pipe(end);
	vars->first_child = fork();
	if (vars->first_child < 0)
	{
		perror("Fork");
		return (FAIL);
	}
	if (vars->first_child == 0)
		first_child(file1, end, vars);
	vars->second_child = fork();
	if (vars->second_child < 0)
	{
		perror("Fork");
		return (FAIL);
	}
	if (vars->second_child == 0)
		second_child(file2, end, vars);
	close(end[0]);
	close(end[1]);
	waitpid(vars->first_child, &status, 0);
	waitpid(vars->second_child, &status, 0);
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
	vars->path = get_binaries_path(env);
	vars->argv = argv;
	file1 = open(argv[1], O_RDONLY);
	file2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC);
	if (file1 < 0 || file2 < 0)
	{
		perror(argv[1]);
		return (free_arg(vars->path));
	}
	if (pipex(file1, file2, vars) == FAIL)
		return (free_arg(vars->path));
	free_arg(vars->path);
	return (SUCCESS);
}
