/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:28:01 by jarumuga          #+#    #+#             */
/*   Updated: 2024/10/16 20:15:22 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// print export
static void	b_export_print(t_hashtable *ht, char **keys, int count, \
t_shell *shell)
{
	int		i;
	char	*value;

	i = -1;
	while (++i < count)
	{
		value = hashtable_search(ht, keys[i])->value;
		ft_putstr_fd("declare -x ", 1);
		if ((ft_strcmp(keys[i], "PATH") == 0) && shell->no_env)
			continue ;
		ft_putstr_fd(keys[i], 1);
		if (value != NULL)
		{
			ft_putstr_fd("=", 1);
			ft_putstr_fd("\"", 1);
			ft_putstr_fd(value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putstr_fd("\n", 1);
	}
}

// env custom
static void	b_export_env(t_shell *shell)
{
	char	**keys;
	int		i;
	int		count;

	if (shell->ht == NULL || shell->ht->count == 0)
		return (ft_putstr_fd("no env var available\n", 2), (void)0);
	keys = (char **)malloc(sizeof(char *) * (shell->ht->count + 1));
	if (!keys)
		return (ft_putstr_fd("malloc failed\n", 2), (void)0);
	count = 0;
	i = -1;
	while (++i < shell->ht->size)
	{
		if (shell->ht->items[i] && !shell->ht->items[i]->tombstone)
			keys[count++] = shell->ht->items[i]->key;
	}
	keys[count] = NULL;
	bubble_sort(keys, count, compare_strings);
	i = -1;
	b_export_print(shell->ht, keys, count, shell);
	free(keys);
}

// key can only be abc, ABC or _
static bool	is_valid_id(const char *s)
{
	int	i;

	i = 1;
	if (!s || !(*s))
		return (false);
	if (!(ft_isalpha(s[0]) || s[0] == '_'))
		return (false);
	while (s[i])
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

// print error when no valid id
static void	print_error_key(char *key, char *value, t_shell *shell)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(key, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	shell->exit_code = 1;
	if (ft_strchr(key, '='))
		key = NULL;
	if (key != NULL)
		free(key);
	if (value != NULL)
		free(value);
}

// export alone -> print custom env
// export key = -> value NULL
// if = sign
void	b_export(char **av, t_shell *shell)
{
	char	*key;
	char	*value;

	value = NULL;
	if (!av[1])
		return (b_export_env(shell), (void)0);
	if (!ft_strchr(av[1], '='))
		key = ft_strdup(av[1]);
	else
	{
		if (ft_strchr(av[1], '=') == av[1])
			return (print_error_key("=", value, shell), (void)0);
		key = ft_strndup(av[1], ft_strchr(av[1], '=') - av[1]);
		value = ft_strdup(ft_strchr(av[1], '=') + 1);
	}
	if (!is_valid_id(key))
		return (print_error_key(key, value, shell), (void)0);
	if (hashtable_get(shell->ht, key) != NULL && value == NULL)
		return (free(key), (void)0);
	if (hashtable_get(shell->ht, key) != NULL)
		hashtable_delete(shell->ht, key);
	if (!hashtable_insert(shell->ht, key, value))
		ft_putstr_fd("export: Failed to export variable\n", 2);
	shell->exit_code = 0;
}
