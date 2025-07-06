/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:44:05 by mdakni            #+#    #+#             */
/*   Updated: 2025/07/06 11:19:01 by mdakni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_manager *manager, int i)
{
	long	check_time;

	pthread_mutex_lock(&manager->philos[i].time_lock);
	check_time = ft_time(manager) - manager->philos[i].last_ate;
	pthread_mutex_unlock(&manager->philos[i].time_lock);
	if (check_time > manager->time_to_die)
	{
		pthread_mutex_lock(&manager->death_check);
		manager->died = true;
		printf("%ld %d died\n", ft_time(manager), i);
		pthread_mutex_unlock(&manager->death_check);
		return (1);
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_manager	*manager;
	int			i;

	manager = (t_manager *)arg;
	while (manager->all_ready == false)
		;
	while (1)
	{
		i = 0;
		while (i < manager->n_philos)
		{
			if (check_death(manager, i) == 1)
				return (NULL);
			pthread_mutex_lock(&manager->all_philos_ate);
			if (manager->eat_times != -1
				&& manager->philos_ate == manager->n_philos)
				return (pthread_mutex_unlock(&manager->philos[i].ate_number),
					NULL);
			pthread_mutex_unlock(&manager->all_philos_ate);
			i++;
		}
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo			*philo;
	struct timeval	tv;

	philo = (t_philo *)arg;
	while (philo->manager->all_ready == false)
		;
	gettimeofday(&tv, NULL);
	if (philo->index % 2 == 0)
	{
		ft_print(philo, "is sleeping");
		if (ft_sleep(philo, philo->manager->time_to_sleep) == 1)
			return (NULL);
	}
	while (1)
	{
		ft_print(philo, "is thinking");
		if (philo->manager->n_philos == 1 && ft_eat_single(philo) == 1)
			return (NULL);
		else if (philo->manager->n_philos != 1 && ft_eat(philo) == 1)
			return (NULL);
		ft_print(philo, "is sleeping");
		if (ft_sleep(philo, philo->manager->time_to_sleep) == 1)
			return (NULL);
	}
	return (NULL);
}

void	destroy_mutex(t_manager *manager)
{
	int	i;

	i = 0;
	while (i < manager->n_philos)
	{
		pthread_mutex_destroy(&(manager->forks[i]));
		pthread_mutex_destroy(&(manager->philos[i].time_lock));
		pthread_mutex_destroy(&(manager->philos[i].ate_number));
		i++;
	}
	pthread_mutex_destroy(&manager->death_check);
	pthread_mutex_destroy(&manager->all_philos_ate);
	free(manager->philos);
	free(manager->forks);
}

int	main(int ac, char **av)
{
	t_manager	manager;
	int			i;

	if (ft_init(&manager, ac, av) == 1)
		return (1);
	i = 0;
	while (i < manager.n_philos)
	{
		pthread_create(&(manager.philos[i].thread), NULL, &routine,
			&(manager.philos[i]));
		i++;
	}
	pthread_create(&manager.monitor, NULL, &monitor, &manager);
	manager.all_ready = true;
	i = 0;
	while (i < manager.n_philos)
	{
		pthread_join(manager.philos[i].thread, NULL);
		i++;
	}
	pthread_join(manager.monitor, NULL);
	destroy_mutex(&manager);
	return (0);
}
