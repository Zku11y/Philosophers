/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:05:46 by mdakni            #+#    #+#             */
/*   Updated: 2025/07/06 14:06:50 by mdakni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_print(t_manager *manager, char *str)
{
	sem_wait(manager->death_check);
	printf("%ld %d %s\n", ft_time(manager), manager->index, str);
	sem_post(manager->death_check);
}

void	ft_eat(t_manager *manager)
{
	sem_wait(manager->sem);
	ft_print(manager, "has taken a fork");
	sem_wait(manager->sem);
	ft_print(manager, "has taken a fork");
	sem_wait(manager->time_lock);
	manager->last_ate = ft_time(manager);
	sem_post(manager->time_lock);
	ft_print(manager, "is eating");
	usleep(manager->time_to_eat * 1000);
	manager->times_ate++;
	if (manager->eat_times != -1 && manager->times_ate > manager->eat_times)
		exit(0);
	sem_post(manager->sem);
	sem_post(manager->sem);
}

void	ft_clean(t_manager *manager)
{
	sem_close(manager->sem);
	sem_close(manager->death_check);
	sem_close(manager->time_lock);
	sem_unlink("/philo_sem");
	sem_unlink("/death_check");
	sem_unlink("/time_lock");
	free(manager->pids);
}

void	ft_fork(t_manager *manager)
{
	pid_t	pid;

	manager->index = 0;
	while (manager->index < manager->n_philos)
	{
		pid = fork();
		if (pid == 0)
		{
			philosophy(manager);
			exit(0);
		}
		else if (pid > 0)
		{
			manager->pids[manager->index] = pid;
			manager->index++;
		}
		else
		{
			ft_clean(manager);
			exit(1);
		}
	}
}

void	ft_wait(t_manager *manager)
{
	int	status;

	wait(&status);
	manager->index = 0;
	while (manager->index < manager->n_philos)
	{
		kill(manager->pids[manager->index], SIGKILL);
		manager->index++;
	}
	manager->index = 0;
	while (manager->index < manager->n_philos)
		waitpid(manager->pids[manager->index++], NULL, 0);
	ft_clean(manager);
	exit(0);
}
