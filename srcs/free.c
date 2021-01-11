/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paulohl <pohl@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 06:34:05 by paulohl           #+#    #+#             */
/*   Updated: 2020/11/27 10:57:04 by paulohl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

char		**free_argv(char **argv, int argc, char *executable_path)
{
	while (argc-- && argv[argc])
		free(argv[argc]);
	free(argv);
	if (executable_path)
		free(executable_path);
	return (NULL);
}

void		reset_fds(t_command *command)
{
	if (command->fd_in != 0)
		close(command->fd_in);
	if (command->fd_out != 1)
		close(command->fd_out);
}
