/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 12:10:52 by jarumuga          #+#    #+#             */
/*   Updated: 2024/10/09 12:28:17 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// check if the option is one or more 'n'
static bool	is_valid_n(char *av)
{
	int	j;

	j = 1;
	if (av[j] == '\0' || av[j] != 'n')
		return (false);
	while (av[j] == 'n')
		j++;
	if (av[j] == '\0')
		return (true);
	else
		return (false);
}

// turn off newline if option
static bool	parse_options(char **av, int *i)
{
	bool	newline;

	newline = true;
	while (av[*i] && av[*i][0] == '-')
	{
		if (is_valid_n(av[*i]))
		{
			newline = false;
			(*i)++;
		}
		else
			break ;
	}
	return (newline);
}

// print argument
static bool	print_argument(char *arg)
{
	ft_putstr_fd(arg, 1);
	if (errno == ENOSPC)
	{
		write(2, "b_echo: write error: No space left on device\n", 45);
		return (false);
	}
	return (true);
}

// print more than one args
static bool	print_arguments(char **av, int i)
{
	bool	error_occurred;

	error_occurred = false;
	while (av[i] && !error_occurred)
	{
		if (!print_argument(av[i]))
			return (true);
		i++;
		if (av[i] && !error_occurred)
		{
			if (!print_argument(" "))
				return (true);
		}
	}
	return (error_occurred);
}

// print the word
	// newline : if option 'n', it's false
	// error_occured : print an error if you try to print in /dev/full
void	b_echo(char **av, t_shell *shell)
{
	bool	newline;
	int		i;
	bool	error_occurred;

	i = 1;
	newline = parse_options(av, &i);
	error_occurred = print_arguments(av, i);
	if (newline && !error_occurred)
	{
		ft_putstr_fd("\n", 1);
		shell->exit_code = 0;
		if (errno == ENOSPC)
		{
			shell->exit_code = 1;
			write(2, "b_echo: write error: No space left on device\n", 45);
		}
	}
}
