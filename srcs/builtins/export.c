/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nomoon <nomoon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 19:40:48 by nomoon            #+#    #+#             */
/*   Updated: 2021/01/11 16:49:10 by nomoon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "../inc/minishell.h"
#include "../libft/libft.h"

int     check_name(char *str)
{
    int i;

    i = -1;
    if (str[0] == '=')
        return (2);
    while (str[++i] && str[i] != '=')
    {
        if (!ft_isalpha(str[i]))
            return (2);
    }
    i = -1;
    while (str[++i])
    {
        if (str[i] == '=')
            return (0);
    }
    return (1);
}

int     change_value(t_env *env, char *argv)
{
    t_env *tmp;
    
    tmp = env;
    
    while (tmp->next)
    {
        if (!ft_strcmp(ft_substr(argv, 0, len_env(argv)), tmp->name))
        {
            tmp->value = ft_substr(argv, len_env(argv) + 1, 
            ft_strlen(argv) - len_env(argv) - 1);
            return (0);
        }
        tmp = tmp->next;
    }
    return (1);
}

int     existing_name(t_env *env, char *argv)
{
    t_env *tmp;
    
    tmp = env;
    
    while (tmp->next)
    {
        if (!ft_strcmp(ft_substr(argv, 0, len_env(argv)), tmp->name))
            return (0);
        tmp = tmp->next;
    }
    return (1);
}

int     export_check(t_env *env, char **argv)
{
    int q = -1;
	while (argv[++q])
	{
		printf("%s\n", argv[q]);
	}
    int i;

    i = 0;
    if (count_argv(argv) == 1)
            export(env, count_argv(argv), argv[i]);
    while (argv[++i])
    {
        if (check_name(argv[i]) == 2)
        {
            ft_putstr_fd("bash: export: `", 1);
            ft_putstr_fd(argv[i], 1);
            ft_putstr_fd("' : not a valid identifier", 1);
            ft_putstr_fd("\n", 1);
        }
        else if (check_name(argv[i]) == 1 || check_name(argv[i]) == 0)
        {
            if (existing_name(env, argv[i]) == 1)
                export(env, count_argv(argv), argv[i]);
            else if (existing_name(env, argv[i]) == 0 && check_name(argv[i]) == 0)
                change_value(env, argv[i]);
        }
    }
    return (1);
}

int     export(t_env *env, int argc, char *argv)
{
    t_env *tmp;
    t_env *temp;

    tmp = env;
    if (argc == 1)
        print_env_alhasort(env);
    else
    {
        while (tmp->next)
            tmp = tmp->next;
        if (!(temp = malloc(sizeof(t_env))))
            return (NULL);
        if (check_name(argv) == 1)
            temp->aff = 1;
        temp->name = tmp->name;
        temp->value = tmp->value;
        tmp->name = ft_substr(argv, 0, len_env(argv));
        tmp->value = ft_substr(argv, len_env(argv) + 1,
        ft_strlen(argv) - len_env(argv) - 1);
        tmp->next = temp;
    }
    return (1);
}