#include "minishell.h"

bool	nothing_to_parse(char *str, char c)
{
	if (!str || !*str)
	{
		printf("Error: nothing to parse after %c\n", c);
		return (true);
	}
	while (*str && is_whitespace(*str))
		str++;
	if (*str)
		return (false);
	printf("Error: nothing to parse after %c\n", c);
	return (true);
}

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\r' || c == '\f')
		return (1);
	return (0);
}

int	is_delimiter(char c)
{
	if (c == '(' || c == ')' || c == '&' || c == '|' || c == 0)
		return (1);
	return (0);
}

int	skip_whitespace(char **str)
{
	char	*s;

	s = *str;
	while (*(*str) && (*(*str) == ' ' || *(*str) == '\t'
		|| *(*str) == '\r' || *(*str) == '\n' || *(*str) == '\f'))
	{
		(*str)++;
	}
	return ((int)(*str - s));
}

void	skip_quotes(char **str)
{
	while (*(*str) && !is_delimiter(*(*str)))
	{
		if (*(*str) == '\'')
		{
			(*str)++;
			while (*(*str) && *(*str) != '\'')
				(*str)++;
		}
		if (*(*str) == '\"')
		{
			(*str)++;
			while (*(*str) && *(*str) != '\"')
				(*str)++;
		}
		(*str)++;
	}
}
