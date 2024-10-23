/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 20:05:43 by habernar          #+#    #+#             */
/*   Updated: 2024/10/15 21:58:23 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_env(t_shell *shell)
{
	int		i;
	int		u;
	char	**tab;
	t_item	*item;

	i = 0;
	u = 0;
	tab = (char **)malloc(sizeof(char *) * shell->ht->count);
	if (!tab)
		return (shell->env = 0, shell->exit_code = 1, exit_shell(shell));
	while (i < shell->ht->size)
	{
		item = shell->ht->items[i];
		if (item && item->key && item->value)
			tab[u++] = ft_strjoin_slash(item->key, item->value, '=');
		i++;
	}
	tab[u] = 0;
	shell->env = tab;
}

static void	create_env(t_shell *shell)
{
	char	buffer[100];
	char	*tmp;

	shell->no_env = true;
	hashtable_insert(shell->ht, ft_strdup("SHLVL"), ft_strdup("1"));
	hashtable_insert(shell->ht, ft_strdup("_"), ft_strdup(ENV));
	hashtable_insert(shell->ht, ft_strdup("PATH"), ft_strdup(PATH));
	tmp = getcwd(buffer, 100);
	if (!tmp)
		return (shell->env = 0, shell->exit_code = 1, exit_shell(shell));
	hashtable_insert(shell->ht, ft_strdup("PWD"), ft_strdup(tmp));
	fill_env(shell);
}

static void	init_env(t_shell *shell, char **env)
{
	int		i;
	char	*k;
	char	*v;

	if (!*env)
		create_env(shell);
	else
	{
		shell->env = env;
		i = 0;
		while (shell->ht && env && env[i])
		{
			k = ft_strndup(env[i], ft_strchr(env[i], '=') - env[i]);
			v = ft_strdup(ft_strchr(env[i], '=') + 1);
			hashtable_insert(shell->ht, k, v);
			i++;
		}
	}
}

void	init_shell(t_shell *shell, int argc, char **argv, char **env)
{
	shell->argc = argc;
	shell->argv = argv;
	shell->exit_code = 0;
	shell->parse_error = 0;
	shell->cl = 0;
	shell->headcl = 0;
	shell->ast = 0;
	shell->no_env = false;
	shell->ht = hashtable_init();
	shell->env = env;
	init_env(shell, env);
	setup_signal();
}
