/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcollas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 23:38:48 by rcollas           #+#    #+#             */
/*   Updated: 2021/09/20 23:38:49 by rcollas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	pid_t	first_child;
	pid_t	second_child;
} 			t_var;

#endif
