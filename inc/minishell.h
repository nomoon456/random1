/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nomoon <nomoon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/16 19:39:29 by paulohl           #+#    #+#             */
/*   Updated: 2021/01/11 17:11:10 by nomoon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_CMD_LEN 262144
# define PIPE_YES 'y'
# define PIPE_NO 'n'
# define GLOB_SUCCESS 0
# define GLOB_ERR_MALLOC 1
# define GLOB_CMD_LEN 2

# include "debug.h"

/*
** s_command is the structure containing information about each issued command.
** cmd contains the raw command to be interpreted by parse_command,
** pipe is set to PIPE_YES whenever there is a '|' at the end of the command,
** type_in and type_out are a character indicating the type of redirection :
** ';' (no redirection), '|', '<', '>' or 'a' ('>>'),
** fd_in and fd_out contain the file descriptor to write to.
** return_value is a variable set to the return value of each launched command,
** it is set to errno when an error is encountered.
*/

typedef struct			s_env {
	char				*name;
	char				*value;
	int					aff;
	int					sort;
	// void				*next;
	struct				s_env *next;
}						t_env;

typedef struct			s_command {
	char				*cmd;
	char				pipe;
	char				type_in;
	char				type_out;
	int					fd_in;
	int					fd_out;
	int					return_value;
	int					pipe_fd[2];
}						t_command;

int						parser(char *buffer, t_command *command, char **envp, t_env *env);
int						skip_quote(char *buffer, int i);
int						skip_redirect(char *buffer, int i);
char					**parse_command(t_command *cmd, int argc, char **envp);
char					*get_word(t_command *cmd, int st, int end, char **envp);
char					*get_env(t_command *cmd, int *i, char **env);
char					**free_argv(char **argv, int argc, char *exec_path);
int						basic_syntax_check(char *str);
void					print_syntax_error(int err);
void					print_parser_error(int err, t_command *command);
t_command				*init_struct(void);
void					set_redirect(t_command *command, char type, int fd);
void					reset_fds(t_command *command);
char					*get_executable_path(char *program_name, char **envp);
int						execute(char *p, t_command *cmd, char **av, char **envp, t_env *env);
char					*get_word(t_command *cmd, int i, int start, char **env);
char					*get_env_str(char *str, char **envp);
int						ft_strcpy_free_input(char *dst, char *src, int to_free);
int						is_builtin(char *path);
t_env     				*stock_env(t_env *env, char **envp);
void    				print_env(t_env *env, int ind);
int     				len_env(char *str);
int     				count_argv(char **argv);
int     				export(t_env *env, int argc, char *argv);
int						ft_pwd(void);
int						builtin_exec(char *path, t_command *cmd, char **argv, t_env *env);
int     				unset(t_env *env, char **argv);
void    print_env_alhasort(t_env *env);
int				ft_echo(t_env *env, char **args);
int    export_check(t_env *env, char **argv);
int             is_in_env(t_env *env, char *str);
char			*get_oldpwd(t_env *env);
int		update_oldpwd(t_env *env);
int		update_pwd(t_env *env);
int				ft_cd(char **args, t_env *env);
int     change_value(t_env *env, char *argv);

#endif
