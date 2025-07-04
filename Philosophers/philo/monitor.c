/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:00:56 by mel-adna          #+#    #+#             */
/*   Updated: 2025/05/20 18:49:31 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_philo_death(t_data *data, int i, int is_full)
{
	uint64_t	current_time;

	pthread_mutex_lock(&data->philos[i].lock);
	current_time = get_time();
	if (current_time >= data->philos[i].time_to_die)
	{
		pthread_mutex_lock(&data->write);
		pthread_mutex_lock(&data->lock);
		pthread_mutex_lock(&data->die);
		if (!data->dead)
		{
			data->dead = 1;
			printf("%llu %d died\n", current_time - data->start_time,
				data->philos[i].id);
		}
		pthread_mutex_unlock(&data->lock);
		pthread_mutex_unlock(&data->die);
		pthread_mutex_unlock(&data->write);
		return (pthread_mutex_unlock(&data->philos[i].lock), 1);
	}
	is_full = data->philos[i].is_full;
	pthread_mutex_unlock(&data->philos[i].lock);
	if (is_full)
		return (2);
	return (0);
}

void	*monitor(void *arg)
{
	t_data	*data;
	int		full_count;
	int		result;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		full_count = 0;
		i = -1;
		while (++i < data->philo_num)
		{
			result = check_philo_death(data, i, 0);
			if (result == 1)
				return (NULL);
			if (result == 2)
				full_count++;
		}
		pthread_mutex_lock(&data->lock);
		if (data->meals_nb != -1 && full_count == data->philo_num)
			return (data->finished = 1, 
				pthread_mutex_unlock(&data->lock), NULL);
		pthread_mutex_unlock(&data->lock);
		usleep(10);
	}
}
