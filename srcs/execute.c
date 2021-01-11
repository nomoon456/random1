/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nomoon <nomoon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 07:19:55 by paulohl           #+#    #+#             */
/*   Updated: 2020/12/28 23:00:13 by nomoon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int		builtin_handler(char *path, t_command *cmd, char **argv, t_env *env)
{
	int		i;

	if (!ft_strcmp(path, "exit"))
	{
		i = 0;
		while (argv[i])
			i++;
		if (i == 1)
			cmd->return_value = 1;
		else if (i == 2)
		{
			i = 0;
			while (is_whitespace(argv[1][i]))
				i++;
			cmd->return_value = ft_isdigit(argv[1][i]) ? ft_atoi(argv[1]) : 255;
		}
		else if ((cmd->return_value = 7) == 7)
			return ((errno = cmd->return_value));
	}
	builtin_exec(path, cmd, argv, env);
	return (0);
}

void	close_pipe(int pipe_fd[2])
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

void	dup_selector(int to_dup[2], t_command *command, int new_pipe_out)
{
	to_dup[0] = 0;
	to_dup[1] = 1;
	if (command->type_in == '|')
		to_dup[0] = command->pipe_fd[0];
	else if (command->type_in == '<')
		to_dup[0] = command->fd_in;
	if (command->type_out == '|')
		to_dup[1] = new_pipe_out;
	else if (command->type_out == '>' || command->type_out == 'a')
		to_dup[1] = command->fd_out;
}

void	slave_action(int to_dup[2], t_command *cmd)
{
	dup2(to_dup[0], 0);
	dup2(to_dup[1], 1);
	if (cmd->type_in == '|')
		close_pipe(cmd->pipe_fd);
}

int		execute(char *path, t_command *cmd, char **argv, char **envp, t_env *env)
{
	int		to_dup[2];
	int		new_pipe[2];
	pid_t	pid;

	if (is_builtin(path))
	{
		printf("1\n");
		return (builtin_handler(path, cmd, argv, env));
	}
	if ((cmd->type_out == '|' || cmd->pipe == PIPE_YES) && pipe(new_pipe))
		return (-1);
	if ((pid = fork()) < 0)
		return (-2);
	dup_selector(to_dup, cmd, new_pipe[1]);
	if (pid == 0)
	{
		slave_action(to_dup, cmd);
		execve(path, argv, envp);
	}
	else
	{
		if (cmd->type_in == '|')
			close_pipe(cmd->pipe_fd);
		waitpid(pid, &cmd->return_value, 0);
		cmd->pipe_fd[0] = new_pipe[0];
		cmd->pipe_fd[1] = new_pipe[1];
	}
	return (0);
}
