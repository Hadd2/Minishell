#include "minishell.h"

static char	*expand_alpha_min(char *str, char *dsign)
{
	char	*s;
	int		i;
	int		size;

	i = 1;
	while (*(dsign + i) && ft_isalpha(*(dsign + i)))
			i++;
	size = ft_strlen(str) - i + 1;
	s = (char *)malloc(sizeof(char) * size);
	if (!s)
		return (0);
	*s = 0;
	ft_strlcat(s, str, dsign - str + 1);
	ft_strlcat(s, dsign + i, size);
	return (s);
}

static char	*expand_alpha_maj(t_shell *shell, char *str, char *dsign)
{
	char	*s;
	char	*key;
	int		i;
	t_item	*item;

	i = 1;
	while (*(dsign + i) && ft_isalpha(*(dsign + i)))
		i++;
	key = ft_strndup(dsign + 1, i - 1);
	if (!key)
		return (0);
	item = hashtable_search(shell->ht, key);
	free(key);
	if (!item && !item->value)
		return (expand_alpha_min(str, dsign));
	s = (char *)malloc(sizeof(char) * (ft_strlen(str) - i + ft_strlen(item->value) + 1));
	if (!s)
		return (0);
	*s = 0;
	ft_strlcat(s, str, dsign - str + 1);
	ft_strlcat(s, item->value, ft_strlen(s) + ft_strlen(item->value) + 1);
	ft_strlcat(s, dsign + i, ft_strlen(s) + ft_strlen(dsign + i) + 1);
	return (s);
}	

static char	*expand_exit_value(t_shell *shell, char *str, char *dsign)
{
	char	*s;
	char	*tmp;
	int		size;

	tmp = ft_itoa(shell->exit_code);
	size = ft_strlen(str) - 2 + ft_strlen(tmp) + 1;
	s = (char *)malloc(sizeof(char) * size);
	if (!s)
	{
		printf("Error: malloc failed at %d, in function %s in file %s\n", __LINE__, __FUNCTION__, __FILE__);
		return (0);
	}
	*s = 0;
	ft_strlcat(s, str, dsign - str + 1);
	ft_strlcat(s, tmp, size);
	ft_strlcat(s, dsign + 2, size);
	free(tmp);
	return (s);
}

static char	*expanded_string(t_shell *shell, char *str, char *dsign)
{
	if (*(dsign + 1) && *(dsign + 1) == '?')
		return (expand_exit_value(shell, str, dsign));
	else if (*(dsign + 1)  && ft_isalpha(*(dsign + 1)))
	{
		if (only_capital_letter(dsign + 1))
			return (expand_alpha_maj(shell, str, dsign));
		else
			return (expand_alpha_min(str, dsign));
	}
	else
		return (0);
}

void	expand_env_variables(t_shell *shell, char **tab)
{
	char	*new;
	char	*dollarsign;
	int		i;

	i = 1;
	while (tab && tab[i])
	{
		dollarsign = ft_strchr(tab[i], '$');
		if (dollarsign)
		{
			if (tab[1][0] != '\'')
			{
				new = expanded_string(shell, tab[i], dollarsign);
				if (new)
				{
					free(tab[i]);
					tab[i] = new;
					continue;
				}
			}
		}
		i++;
	}
}
