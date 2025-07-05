/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skully <skully@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:02:09 by mdakni            #+#    #+#             */
/*   Updated: 2025/06/28 18:36:48 by skully           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	unsigned char	*ptr;

	i = 0;
	ptr = (unsigned char *)b;
	while (i < len)
	{
		ptr[i] = (unsigned char)c;
		i++;
	}
	return (b);
}

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*ptr;

	if (count == 0 || size == 0)
	{
		ptr = NULL;
		return (ptr);
	}
	ptr = malloc(count * size);
	if (ptr == NULL)
		return (NULL);
	ft_memset(ptr, '\0', size * count);
	return (ptr);
}

int	ft_atoi(const char *str)
{
	int			i;
	long long	answer;
	int			sign;

	i = 0;
	answer = 0;
	sign = 1;
	while ((str[i] == ' ') || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while ((str[i] >= '0' && str[i] <= '9'))
		answer = (answer * 10) + (str[i++] - '0');
	if (answer > LONG_MAX)
	{
		if (sign == 1)
			return (-1);
		return (0);
	}
	return ((int)(answer * sign));
}
