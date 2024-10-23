/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 19:29:34 by jarumuga          #+#    #+#             */
/*   Updated: 2024/10/09 12:27:42 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// change to the home directory
// if the HOME variable is not set, we print an error message
static void	change_to_home(t_hashtable *ht, t_shell *shell)
{
	char	*home;

	home = hashtable_get(ht, "HOME");
	if (home)
	{
		if (chdir(home) != 0)
		{
			perror("cd: error changing directory to HOME");
			shell->exit_code = 1;
		}
	}
	else
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		shell->exit_code = 1;
	}
}

// change to the old directory
// if chdir is successful, we print the new directory
// if the OLDPWD variable is not set, we print an error message
static void	change_to_oldpwd(t_hashtable *ht, t_shell *shell, char **av)
{
	char	*old_pwd;
	char	buffer[1024];

	(void)av;
	old_pwd = hashtable_get(ht, "OLDPWD");
	if (old_pwd)
	{
		if (chdir(old_pwd) == 0)
		{
			ft_putstr_fd(getcwd(buffer, sizeof(buffer)), 1);
			ft_putstr_fd("\n", 1);
		}
		else
		{
			perror("cd: error changing directory to OLDPWD");
			shell->exit_code = 1;
		}
	}
	else
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		shell->exit_code = 1;
	}
}

// getting the current directory
// if the PWD variable is not set, we get the current directory using getcwd
static void	get_curr_dir(t_hashtable *ht, t_shell *shell)
{
	char	*pwd;

	pwd = hashtable_get(ht, "PWD");
	if (!pwd)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
		{
			perror("cd: error getting current directory");
			return (shell->exit_code = 1, (void)0);
		}
		hashtable_insert(ht, ft_strdup("PWD"), pwd);
	}
}

// update the PWD and OLDPWD variables
static void	update_env_var(t_hashtable *ht, t_shell *shell)
{
	char	*current_pwd;
	char	*old_pwd;
	char	buffer[1024];

	(void)shell;
	current_pwd = hashtable_get(ht, "PWD");
	old_pwd = hashtable_get(ht, "OLDPWD");
	if (old_pwd == NULL)
		hashtable_insert(ht, ft_strdup("OLDPWD"), ft_strdup(""));
	else
		hashtable_insert(ht, "OLDPWD", current_pwd);
	if (!getcwd(buffer, sizeof(buffer)))
	{
		ft_putstr_fd("cd: error retrieving current directory: ", 2);
		ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
		shell->exit_code = 1;
	}
	else
		hashtable_insert(ht, "PWD", getcwd(buffer, sizeof(buffer)));
}

// change the current directory
void	b_cd(char **av, t_hashtable *ht, t_shell *shell)
{
	get_curr_dir(ht, shell);
	if (av[1] == NULL || ft_strcmp(av[1], "~") == 0)
		change_to_home(ht, shell);
	else if (ft_strcmp(av[1], "-") == 0)
		change_to_oldpwd(ht, shell, av);
	else if (av[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		shell->exit_code = 1;
	}
	else
	{
		if (chdir(av[1]) != 0)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			perror(av[1]);
			shell->exit_code = 1;
		}
	}
	update_env_var(ht, shell);
}
