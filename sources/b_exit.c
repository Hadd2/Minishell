/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:53:46 by jarumuga          #+#    #+#             */
/*   Updated: 2024/10/09 12:28:42 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// check if char is a number
	// if - or + skip
	// if 0, return 0
	// return 0, if it's not a number
	// return 1, if ok
static int	ft_isnumeric(const char *str)
{
	if (*str == '-' || *str == '+')
		str++;
	if (*str == '\0')
		return (0);
	while (*str)
	{
		if (*str == '"' || *str == '\"')
		{
			str++;
			continue ;
		}
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

// check if the number overflow the long long
	// if it's exactly long long min, return 0
	// if it's higher than long long max or min, return -1, and affect it
	// return 0 if no overflow
static int	ft_overflow(const char **str, long long *res, \
int sign, long long *result)
{
	while (ft_isdigit((unsigned char)**str))
	{
		if (sign == -1 && *res == LLONG_MAX / 10 && (**str - '0') == 8)
		{
			*result = LLONG_MIN;
			(*str)++;
			return (0);
		}
		if (sign == 1 && *res > (LLONG_MAX - (**str - '0')) / 10)
		{
			*result = LLONG_MAX;
			return (-1);
		}
		else if (sign == -1 && *res > (LLONG_MAX - (**str - '0')) / 10)
		{
			*result = LLONG_MIN;
			return (-1);
		}
		*res = *res * 10 + (**str - '0');
		(*str)++;
	}
	return (0);
}

// ascii to long long
	// if str is null return 0
	// if is_whitespace skip
	// if nbr neg, sign -1 and skip
	// if +, skip
	// if overflow return -1
	// affect res * sign in argument *result
static int	ft_atoll(const char *str, long long *result)
{
	long long	res;
	int			sign;

	res = 0;
	sign = 1;
	if (str == NULL)
		return (0);
	while (is_whitespace((unsigned char)*str))
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str == '\'' || *str == '"')
		str++;
	if (ft_overflow(&str, &res, sign, result) == -1)
		return (-1);
	*result = res * sign;
	return (0);
}

// error message if it's not a nbr or overflow
	// print error
	// exit_code 2
	// exit
static void	numeric_av_req(char **av, t_shell *shell)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(av[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	shell->exit_code = 2;
	exit_shell(shell);
}

// exit function
	// print exit
	// if no exit_code, exit
	// if not a nbr or overflow, print an error and exit
	// if too may args, print error
	// if exit_code, print modulo of 256, check with echo $?
void	b_exit(char **av, t_shell *shell)
{
	long long	num;

	ft_putstr_fd("exit\n", 1);
	if (av[1] == NULL)
		exit_shell(shell);
	else if (!ft_isnumeric(av[1]) || ft_atoll(av[1], &num) == -1)
		numeric_av_req(av, shell);
	else if (av[2] != NULL)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->exit_code = 1;
	}
	else
	{
		shell->exit_code = (int)(num % 256);
		exit_shell(shell);
	}
}
