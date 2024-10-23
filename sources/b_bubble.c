/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_bubble.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:42:45 by jarumuga          #+#    #+#             */
/*   Updated: 2024/10/09 14:54:32 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// swap
static void	ft_swap(void **a, void **b)
{
	void	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

// compare two strings
int	compare_strings(const void *a, const void *b)
{
	const char	*str1;
	const char	*str2;

	str1 = *(const char **)a;
	str2 = *(const char **)b;
	return (ft_strcmp(str1, str2));
}

// sort an array, compare the first string with the second one
void	bubble_sort(char **array, int size, \
int (*cmp)(const void *, const void *))
{
	int	i;
	int	j;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (cmp(&array[j], &array[j + 1]) > 0)
				ft_swap((void **)&array[j], (void **)&array[j + 1]);
			j++;
		}
		i++;
	}
}
