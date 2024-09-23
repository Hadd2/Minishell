/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:43:21 by habernar          #+#    #+#             */
/*   Updated: 2024/09/23 14:52:02 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hashtable_delete(t_hashtable *ht, const char *k)
{
	t_item	*item;
	int		i;
	int		attemp;

	if (ht->count == 0)
		return ;
	i = hash(k) % ht->size;
	attemp = 0;
	while (attemp < ht->size)
	{
		item = ht->items[i];
		if (!item)
			return ;
		if (!item->tombstone && !ft_strncmp(item->key, k, ft_strlen(k)))
		{
			item->tombstone = true;
			ht->count--;
			return ;
		}
		i = (i + 1) % ht->size;
		attemp++;
	}
}

t_item	*hashtable_search(t_hashtable *ht, const char *k)
{
	t_item		*entry;
	uint32_t	i;
	int			attemp;

	if (ht->count == 0)
		return (0);
	i = hash(k) % ht->size;
	attemp = 0;
	while (attemp < ht->size)
	{
		entry = ht->items[i];
		if (!entry)
			return (0);
		if (!entry->tombstone && !ft_strncmp(entry->key, k, ft_strlen(k)))
			return (entry);
		i = (i + 1) % ht->size;
		attemp++;
	}
	return (0);
}

void	hashtable_resize(t_hashtable *ht)
{
	t_item	**new_table;
	int		i;
	int		j;

	new_table = (t_item **)malloc(sizeof(t_item *) * ht->size * 2);
	if (!new_table)
	{
		perror("malloc");
		return ;
	}
	i = 0;
	while (i < ht->size * 2)
		new_table[i++] = 0;
	i = 0;
	while (i < ht->size)
	{
		if (ht->items[i] && !ht->items[i]->tombstone)
		{
			j = hash(ht->items[i]->key) % (ht->size * 2);
			while (new_table[j])
				j = (j + 1) % (ht->size * 2);
			new_table[j] = ht->items[i];
		}
		else if (ht->items[i] && ht->items[i]->tombstone)
			item_delete(ht->items[i]);
		i++;
	}
	free(ht->items);
	ht->items = new_table;
	ht->size *= 2;
}

bool	hashtable_insert(t_hashtable *ht, char *k, char *v)
{
	t_item		*entry;
	uint32_t	i;

	if (ht->count + 1 > ht->size * HT_MAX_LOAD)
		hashtable_resize(ht);
	entry = hashtable_search(ht, k);
	if (entry)
	{
		free(entry->value);
		entry->value = strdup(v);
		entry->tombstone = false;
		return (true);
	}
	entry = item_create(k, v);
	if (!entry)
		return (false);
	i = hash(k) % ht->size;
	while (ht->items[i])
		i = (i + 1) % ht->size;
	ht->items[i] = entry;
	ht->count++;
	return (true);
}
