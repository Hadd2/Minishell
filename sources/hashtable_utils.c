#include "minishell.h"

t_item	*item_create(char *k, char *v)
{
    t_item	*item;

    item = (t_item *)malloc(sizeof(t_item));
    if (!item)
    {
        printf("Error: malloc failed at line %d, in function %s in file %s\n", __LINE__, __FUNCTION__, __FILE__);
        return (0);
    }
    item->key = k; 
    item->value = v;
	item->tombstone = false;
    return (item);
}

void	item_delete(t_item *item)
{
    free(item->key);
    free(item->value);
    free(item);
}


t_hashtable	*hashtable_init(void)
{
    t_hashtable	*ht;
    int			i;

    ht = (t_hashtable *)malloc(sizeof(t_hashtable));
    if (!ht)
    {
        printf("Error: malloc failed at line %d, in function %s in file %s\n", __LINE__, __FUNCTION__, __FILE__);
        return (0);
    }
    ht->size = 43;
    ht->count = 0;
    ht->items = (t_item **)malloc(sizeof(t_item *) * ht->size);
    if (!ht->items)
    {
        printf("Error: malloc failed at line %d, in function %s in file %s\n", __LINE__, __FUNCTION__, __FILE__);
        free(ht);
        return (0);
    }
    for (i = 0; i < ht->size; i++)
        ht->items[i] = 0;
    return (ht);
}

void	hashtable_free(t_hashtable *ht)
{
	int	i;

	i = 0;
	while (i < ht->size)
	{
		if (ht->items[i])
		{
			item_delete(ht->items[i]);
		}
		i++;
	}
	free(ht->items);
	free(ht);
	ht = 0;
}

uint32_t	hash(const char *key)
{
    uint32_t	hash;
    uint32_t	i;
    uint8_t		len;

    hash = 2166136261u;
    len = strlen(key);
    i = 0;
    while (i < len)
    {
        hash ^= *(key + i);
        hash *= 1677619;
        i++;
    }
    return (hash);
}
