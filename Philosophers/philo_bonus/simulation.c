/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skully <skully@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 11:08:34 by mel-adna          #+#    #+#             */
/*   Updated: 2025/07/01 12:57:51 by skully           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	eating(t_philo *philo)
{
	t_args	*args;

	args = philo->args;
	sem_wait(args->forks);
	print_status(philo, "has taken a fork");
	sem_wait(args->forks);
	print_status(philo, "has taken a fork");
	sem_wait(args->lock);
	philo->last_meal = get_time();
	sem_post(args->lock);
	print_status(philo, "is eating");
	my_usleep(args->t_eat, philo);
	sem_post(args->forks);
	sem_post(args->forks);
	if (args->must_eat != -1)
	{
		philo->eat_count++;
		if (philo->eat_count >= args->must_eat)
			sem_post(args->meal_gate);
	}
}

void	*check_death(void *arg)
{
	t_philo	*philo;
	t_args	*args;

	philo = (t_philo *)arg;
	args = philo->args;
	while (1)
	{
		sem_wait(args->lock);
		if (get_time() - philo->last_meal >= args->t_die)
		{
			sem_wait(args->print_lock);
			printf("%ld %d died\n", (get_time() - args->start_time),
				philo->id);
			exit(1);
		}
		sem_post(args->lock);
		usleep(10);
	}
	return (NULL);
}

void	philo_routine(t_philo *philo)
{
	t_args		*args;
	pthread_t	death_thread;

	args = philo->args;
	pthread_create(&death_thread, NULL, &check_death, philo);
	pthread_detach(death_thread);
	while (1)
	{
		print_status(philo, "is thinking");
		eating(philo);
		if (args->must_eat != -1 && philo->eat_count > args->must_eat)
			exit(0);
		print_status(philo, "is sleeping");
		my_usleep(args->t_sleep, philo);
	}
}

void	philo_children(t_args *args, pid_t *pids, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < args->n_philo)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			philos[i].id = i + 1;
			philos[i].args = args;
			philos[i].last_meal = args->start_time;
			philos[i].eat_count = 0;
			philo_routine(&philos[i]);
		}
		else if (pids[i] < 0)
		{
			printf("Error: Fork failed\n");
			exit(1);
		}
		i++;
	}
}
