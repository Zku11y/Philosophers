/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:44:05 by mdakni            #+#    #+#             */
/*   Updated: 2025/07/06 14:30:49 by mdakni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitor(void *arg)
{
	t_manager	*manager;
	long		check_time;

	manager = (t_manager *)arg;
	while (1)
	{
		sem_wait(manager->time_lock);
		check_time = ft_time(manager) - manager->last_ate;
		if (check_time >= manager->time_to_die)
		{
			sem_wait(manager->death_check);
			printf("%ld %d died\n", ft_time(manager), manager->index);
			exit(1);
		}
		sem_post(manager->time_lock);
	}
	return (NULL);
}

int	philosophy(t_manager *manager)
{
	manager->times_ate = 0;
	manager->last_ate = ft_time(manager);
	pthread_create(&manager->monitor, NULL, &monitor, manager);
	pthread_detach(manager->monitor);
	if (manager->index % 2 == 0)
	{
		printf("%ld %d is sleeping\n", ft_time(manager), manager->index);
		usleep(manager->time_to_sleep * 1000);
	}
	while (1)
	{
		ft_print(manager, "is thinking");
		ft_eat(manager);
		ft_print(manager, "is sleeping");
		usleep(manager->time_to_sleep * 1000);
	}
	return (exit(EXIT_SUCCESS), 0);
}

int	main(int ac, char **av)
{
	t_manager	manager;

	if (ac > 6 || ac < 5)
		return (printf("\e[1;31mError : Invalid number of arguments\e[0m\n"),
			1);
	if (check_args(&manager, ac, av) == -1)
	{
		return (printf("\e[1;33mError : Invalid values in arguments\e[0m\n"),
			1);
	}
	manager.pids = malloc(manager.n_philos * sizeof(pid_t) + 1);
	gettimeofday(&manager.tv, NULL);
	manager.start_time = (manager.tv.tv_sec * 1000) + (manager.tv.tv_usec
			/ 1000);
	sem_unlink("/philo_sem");
	sem_unlink("/death_check");
	sem_unlink("/time_lock");
	manager.sem = sem_open("/philo_sem", O_CREAT | O_EXCL, 0777,
			manager.n_philos);
	manager.death_check = sem_open("/death_check", O_CREAT | O_EXCL, 0777, 1);
	manager.time_lock = sem_open("/time_lock", O_CREAT | O_EXCL, 0777, 1);
	ft_fork(&manager);
	ft_wait(&manager);
	return (0);
}
