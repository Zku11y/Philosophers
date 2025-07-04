/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:00:52 by mel-adna          #+#    #+#             */
/*   Updated: 2025/05/13 18:20:16 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		data->philos[i].id = i + 1;
		data->philos[i].data = data;
		data->philos[i].eat_count = 0;
		data->philos[i].is_full = 0;
		data->philos[i].eating = 0;
		data->philos[i].l_fork = &data->forks[i];
		data->philos[i].r_fork = &data->forks[(i + 1) % data->philo_num];
		if (pthread_mutex_init(&data->philos[i].lock, NULL))
			return (printf("Philo lock init failed\n"), 1);
		i++;
	}
	return (0);
}

int	init_data(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_num);
	if (!data->forks)
		return (printf("Malloc failed\n"), 1);
	i = 0;
	while (i < data->philo_num)
		if (pthread_mutex_init(&data->forks[i++], NULL))
			return (printf("Fork mutex init failed\n"), 1);
	if (pthread_mutex_init(&data->write, NULL)
		|| pthread_mutex_init(&data->lock, NULL)
		|| pthread_mutex_init(&data->die, NULL))
		return (printf("Global mutex init failed\n"), 1);
	data->philos = malloc(sizeof(t_philo) * data->philo_num);
	if (!data->philos)
		return (printf("Philos malloc failed\n"), 1);
	return (0);
}
