/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbouju <elbouju@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 15:45:33 by nomoon            #+#    #+#             */
/*   Updated: 2021/01/12 11:55:54 by elbouju          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "../inc/minishell.h"
#include "../libft/libft.h"

int     len_env(char *str)
{
    int i;

    i = 0;
    while (str[i] && str[i] != '=')
        i++;
    return (i);
}

// void sort_alpha(t_file **begin_list)
// {
//   t_file    *list;
//   char  *tmp;
//   list = *begin_list;
//   if (list)
//     {
//         while (list && list->next)
//         {
//             if (strcmp(list->file_name, list->next->file_name) > 0)
//             {
//                 tmp = list->file_name;
//                 list->file_name = list->next->file_name;
//                 list->next->file_name = tmp;
//                 counter = counter + 1;
//                 printf("swap=%d\n",counter);
//             }
//         list = list->next;
//         }
//     }
// }

char	*ft_strjoin_free(char *s1, char *s2, char state)
{
	char	*str;
	size_t	len_s1;
	size_t	len_s2;

	if (!s2)
		return (s1);
	if (!s1)
		return (s2);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	if (!(str = ft_calloc(len_s1 + len_s2 + 2, sizeof(char))))
		return (NULL);
	ft_memcpy(str, s1, len_s1 + 1);
	ft_memcpy(str + len_s1, s2, len_s2);
	state == 1 || state == 3 ? free(s1) : 0;
	state == 2 || state == 3 ? free(s2) : 0;
	return (str);
}

char    **create_alpha_tab()
{
    char **tab;

    tab = malloc(sizeof(char*) * 10);
    for (int i = 0; i < 9; i++)
        tab[i] = strdup('z' - i);
    tab[9] = 0;
    return (tab);
}

void    sort_tab(char **tab, int total)
{
    int i;
    char    *tmp;
    
    i = -1;

    int j = total;
    while (total != 0)
    {
        i = -1;
        while (tab[++i] && tab[i + 1] && i < total)
        {
            if (ft_strcmp(tab[i], tab[i + 1]) > 0)
            // if (ft_strcmp(ft_substr(tab[i], 0, len_env(tab[i])), ft_substr(tab[i + 1], 0, len_env(tab[i + 1]))) < 0)
            {
                tmp = tab[i];
                tab[i] = tab[i + 1];
                tab[i + 1] = tmp;
            }
        }
        total--;
    }
    i = -1;
    while (tab[++i])
        printf("%s\n", tab[i]);
    free(tab);
}

int    print_env_alhasort(t_env *env)
{
    t_env   *tmp;
    char    **tabsort;
    int total;
    char *tmp1;
    int i;

    i = -1;
    total = 0;
    tmp = env;
    while (tmp)
    {
        total += 1;
        tmp = tmp->next;
    }
    if (!(tabsort = ft_calloc(sizeof(char*), (total + 1))))
        return (0);
    tmp = env;
    printf("%s\n", tmp->name);
    read(0, NULL, 1);
    printf("TOTAL %d\n", total);
    while (tmp)
    {
        tmp1 = ft_strjoin(tmp->name, "=");
        tmp1 = ft_strjoin_free(tmp1, tmp->value, 1);
        tabsort[++i] = tmp1;
        tmp = tmp->next;
    }
    write(1, 'a', 1);
    printf("passwd\n");
    sort_tab(tabsort, total);
}

void    print_env(t_env *env, int ind)
{
    t_env *tmp;
    tmp = env;

    while (tmp)
    {
        // printf("%s -- %d\n", tmp->name, tmp->sort);
        // printf("%d && %d\n", tmp->aff, ind);
        if (tmp->aff == 0)
        {
            // printf("\nnb %d\n", tmp->sort);
            ft_putstr_fd(tmp->name, 1);
            ft_putstr_fd("=", 1);
            ft_putstr_fd(tmp->value, 1);
            write(1, "\n", 1);
        }
        tmp = tmp->next;
    }
}

t_env     *stock_env(t_env *env, char **envp)
{
    int     i;
    t_env   *tmp;
    t_env   *new;

    // if (!envp) IF ENV -i
    if (!(env = (t_env *)malloc(sizeof(t_env))))
        return (NULL);
    tmp = env;
    env->name = ft_substr(envp[0], 0, len_env(envp[0]));
    env->value = ft_substr(envp[0], len_env(envp[0]) + 1,
    ft_strlen(envp[0]) - len_env(envp[0]) - 1);
    i = 0;
    while (envp[++i])
    {
        if (!(new = malloc(sizeof(t_env))))
            return (NULL);
        new->name = ft_substr(envp[i], 0, len_env(envp[i]));
        new->value = ft_substr(envp[i], len_env(envp[i]) + 1,
        ft_strlen(envp[i]) - len_env(envp[i]) - 1);
        new->next = NULL;
        env->next = new;
        env = env->next;
    }
    env->next = NULL;
    return(tmp);
}