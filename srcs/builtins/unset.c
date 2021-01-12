/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nomoon <nomoon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 20:02:14 by nomoon            #+#    #+#             */
/*   Updated: 2020/12/21 18:09:03 by nomoon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "../inc/minishell.h"
#include "../libft/libft.h"

int     exist_check(t_env *env, char **argv)
{
    t_env *tmp;
    t_env *prev;
    int i;

    i = 0;
    prev = env;
    while (argv[++i])
        if (!ft_strcmp(ft_substr(argv[i], 0, len_env(argv[i])), prev->name))
            env = prev;
    prev = prev->next;
    i = 0;
    while (argv[++i])
    {
        tmp = env;
        prev = env->next;   
        while (prev->next)
        {
            if (!ft_strcmp(ft_substr(argv[i], 0, len_env(argv[i])), prev->name))
            {
                tmp->next = prev->next;
                break;
            }
            prev = prev->next;
            tmp = tmp->next;
        }
    }
    return (1);
}

int     unset(t_env *env, char **argv)
{   
    exist_check(env, argv);
}