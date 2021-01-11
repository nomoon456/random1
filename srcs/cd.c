/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbouju <elbouju@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 23:25:31 by nomoon            #+#    #+#             */
/*   Updated: 2021/01/11 11:37:41 by elbouju          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <stdlib.h>
#include "../libft/libft.h"
#include <limits.h>
#include <stdio.h>

static void		print_error(char **args)
{
	ft_putstr_fd("cd: ", 2);
	if (args[2])
		ft_putstr_fd("string not in pwd: ", 2);
	else
	{
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(args[1], 2);
}

char			*get_env_path(t_env *env, const char *var, size_t len)
{
	char	*oldpwd;
	t_env	*tmp;

	tmp = env;
	while (tmp && tmp->next)
	{
		if (!ft_strncmp(tmp->name, var, len))
		{
			if (!(oldpwd = malloc(sizeof(char) * ft_strlen(tmp->value) + 1)))
				return (NULL);
			oldpwd = tmp->value;
			oldpwd[ft_strlen(tmp->value)] = '\0';
			return (oldpwd);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

// static char		*get_env_path(t_env *env, const char *var, size_t len)
// {
// 	printf("var %s\n", var);
// 	printf("len %zu\n", len);
// 	char	*oldpwd;
// 	int		i;
// 	int		j;
// 	int		s_alloc;

// 	while (env && env->next != NULL)
// 	{
// 		if (ft_strncmp(env->name, var, len) == 0)
// 		{
// 			s_alloc = ft_strlen(env->value) + 4;
// 			if (!(oldpwd = malloc(sizeof(char) * s_alloc + 1)))
// 				return (NULL);
// 			i = 0;
// 			j = 0;
// 			while (env->value[i++])
// 			{
// 				if (i > (int)len)
// 					oldpwd[j++] = env->value[i];
// 			}
// 			oldpwd[j] = '\0';
// 			printf("%s\n", oldpwd);
// 			return (oldpwd);
// 		}
// 		env = env->next;
// 	}
// 	return (NULL);
// }

void			env_add(char *oldpwd, t_env *env)
{
	export(env, 2, oldpwd);
}

int             is_in_env(t_env *env, char *str)
{
    t_env *tmp;

	printf("%s", env->name);
	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp("OLDPWD", tmp->name))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

static int		update_oldpwd(t_env *env)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;

	if (getcwd(cwd, PATH_MAX) == NULL)
		return (1);
	if (!(oldpwd = ft_strjoin("OLDPWD=", cwd)))
		return (1);
	if (is_in_env(env, oldpwd) == 0)
		env_add(oldpwd, env);
	// ft_memdel(oldpwd);
	return (0);
}

static int		go_to_path(int option, t_env *env)
{
	int		ret;
	char	*env_path;

	env_path = NULL;
	if (option == 0)
	{
		update_oldpwd(env);
		env_path = get_env_path(env, "HOME", 4);
		if (!env_path)
			ft_putendl_fd("minishell : cd: HOME not set", 2);
		if (!env_path)
			return (1);
	}
	else if (option == 1)
	{
		env_path = get_env_path(env, "OLDPWD", 6);
		if (!env_path)
			ft_putendl_fd("minishell : cd: OLDPWD not set", 2);
		if (!env_path)
			return (1);
		update_oldpwd(env);
	}
	ret = chdir(env_path);
	// ft_memdel(env_path);
	return (ret);
}

int				ft_cd(char **args, t_env *env)
{
	int		cd_ret;

	if (!args[1])
		return (go_to_path(0, env));
	if (ft_strcmp(args[1], "-") == 0)
		cd_ret = go_to_path(1, env);
	else
	{
		update_oldpwd(env);
		cd_ret = chdir(args[1]);
		if (cd_ret < 0)
			cd_ret *= -1;
		if (cd_ret != 0)
			print_error(args);
	}
	return (cd_ret);
}