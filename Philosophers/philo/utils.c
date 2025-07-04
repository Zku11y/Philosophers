/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:01:12 by mel-adna          #+#    #+#             */
/*   Updated: 2025/05/20 20:04:39 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((uint64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

void	my_usleep(uint64_t time_in_ms, t_philo *philo)
{
	uint64_t	start;
	uint64_t	current;

	start = get_time();
	while (1)
	{
		pthread_mutex_lock(&philo->data->die);
		if (philo->data->dead)
		{
			pthread_mutex_unlock(&philo->data->die);
			break ;
		}
		pthread_mutex_unlock(&philo->data->die);
		current = get_time();
		if ((current - start) >= time_in_ms)
			break ;
		usleep(100);
	}
}

void	print_status(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->write);
	if (!philo->data->dead)
	{
		printf("%llu %d %s\n", get_time() - philo->data->start_time, philo->id,
			msg);
	}
	pthread_mutex_unlock(&philo->data->write);
}

int	ft_atoi(const char *str)
{
	int				i;
	int				sign;
	unsigned long	result;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
		if (result > 2147483647)
			return (-1);
	}
	return ((int)(result * sign));
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].lock);
		i++;
	}
	pthread_mutex_destroy(&data->write);
	pthread_mutex_destroy(&data->lock);
	free(data->philos);
	free(data->forks);
}
