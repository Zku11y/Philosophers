/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 10:58:20 by mdakni            #+#    #+#             */
/*   Updated: 2025/07/06 11:17:20 by mdakni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	assign_manager_time(t_manager *manager, int ac, int *val)
{
	if (val[0] < 1 || val[0] > 200 || val[1] <= 0 || val[2] <= 0 || val[3] <= 0
		|| (val[4] != -1 && val[4] <= 0))
		return (-1);
	manager->n_philos = val[0];
	manager->time_to_die = val[1];
	manager->time_to_eat = val[2];
	manager->time_to_sleep = val[3];
	if (ac == 5)
		manager->eat_times = -1;
	else
		manager->eat_times = val[4];
	manager->died = false;
	manager->all_ready = false;
	manager->philos_ate = 0;
	pthread_mutex_init(&manager->death_check, NULL);
	pthread_mutex_init(&manager->all_philos_ate, NULL);
	return (0);
}

int	check_args(t_manager *manager, int ac, char **av)
{
	int	i;
	int	val[6];

	i = 1;
	while (av[i])
	{
		val[i - 1] = ft_atoi(av[i]);
		i++;
	}
	val[i - 1] = -1;
	if (assign_manager_time(manager, ac, val) == -1)
		return (-1);
	return (0);
}

int	ft_init(t_manager *manager, int ac, char **av)
{
	if (ac > 6 || ac < 5)
		return (printf("\e[1;31mError : Invalid number of arguments\e[0m\n"),
			1);
	if (check_args(manager, ac, av) == -1)
	{
		return (printf("\e[1;33mError : Invalid values in arguments\e[0m\n"),
			1);
	}
	if (alloc_philo_fork(manager) == -1)
		return (printf("\e[1;31mError : Memory allocation error\e[0m\n"), 1);
	gettimeofday(&manager->tv, NULL);
	manager->start_time = (manager->tv.tv_sec * 1000) + (manager->tv.tv_usec
			/ 1000);
	init_philo(manager);
	init_forks(manager);
	return (0);
}

void	init_philo(t_manager *manager)
{
	int	i;
	int	n;

	i = 0;
	n = manager->n_philos;
	while (i < n)
	{
		manager->philos[i].index = i;
		manager->philos[i].left = &(manager->forks[i]);
		if (manager->n_philos == 1)
			manager->philos[i].right = NULL;
		else
			manager->philos[i].right = &(manager->forks[(i + 1) % n]);
		manager->philos[i].manager = manager;
		manager->philos[i].last_ate = ft_time(manager);
		manager->philos[i].times_ate = 0;
		pthread_mutex_init(&(manager->philos[i].time_lock), NULL);
		pthread_mutex_init(&(manager->philos[i].ate_number), NULL);
		i++;
	}
}

void	init_forks(t_manager *manager)
{
	int	i;

	i = 0;
	while (i < manager->n_philos)
	{
		pthread_mutex_init(&(manager->forks[i]), NULL);
		i++;
	}
}
