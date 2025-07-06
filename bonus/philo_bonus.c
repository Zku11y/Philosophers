/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:44:05 by mdakni            #+#    #+#             */
/*   Updated: 2025/07/06 10:53:41 by mdakni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long	ft_time(t_manager *manager)
{
	struct timeval	tv;
	long			current_time;

	gettimeofday(&tv, NULL);
	current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (current_time - manager->start_time);
}

int	assign_manager_time(t_manager *manager, int ac, int *val)
{
	if (val[0] == 0 || val[1] == 0 || val[2] == 0 || val[3] == 0 || (val[4] !=
			-1 && val[4] == 0))
	{
		return (-1);
	}
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
	// printf("DEBUG: Philosophy function entered by process %d at %ldms\n",
		manager->index, ft_time(manager));
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
		sem_wait(manager->death_check);
		printf("%ld %d is thinking\n", ft_time(manager), manager->index);
		sem_post(manager->death_check);
		sem_wait(manager->sem);
		sem_wait(manager->death_check);
		printf("%ld %d has taken a fork\n", ft_time(manager), manager->index);
		sem_post(manager->death_check);
		sem_wait(manager->sem);
		sem_wait(manager->death_check);
		printf("%ld %d has taken a fork\n", ft_time(manager), manager->index);
		sem_post(manager->death_check);
		sem_wait(manager->time_lock);
		manager->last_ate = ft_time(manager);
		sem_post(manager->time_lock);
		sem_wait(manager->death_check);
		printf("%ld %d is eating\n", ft_time(manager), manager->index);
		sem_post(manager->death_check);
		usleep(manager->time_to_eat * 1000);
		manager->times_ate++;
		if (manager->eat_times != -1 && manager->times_ate > manager->eat_times)
			exit(0);
		sem_post(manager->sem);
		sem_post(manager->sem);
		sem_wait(manager->death_check);
		printf("%ld %d is sleeping\n", ft_time(manager), manager->index);
		sem_post(manager->death_check);
		usleep(manager->time_to_sleep * 1000);
	}
	return (exit(EXIT_SUCCESS), 0);
}

int	main(int ac, char **av)
{
	t_manager	manager;
	pid_t		pid;
	int			status;

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
	// printf("DEBUG: Starting semaphore setup at 0ms\n");
	sem_unlink("/philo_sem");
	sem_unlink("/death_check");
	sem_unlink("/time_lock");
	manager.sem = sem_open("/philo_sem", O_CREAT | O_EXCL, 0777,
			manager.n_philos);
	manager.death_check = sem_open("/death_check", O_CREAT | O_EXCL, 0777, 1);
	manager.time_lock = sem_open("/time_lock", O_CREAT | O_EXCL, 0777, 1);
	manager.index = 0;
	// printf("DEBUG: Semaphores created at %ldms\n", ft_time(&manager));
	while (manager.index < manager.n_philos)
	{
		// printf("DEBUG: About to fork child %d at %ldms\n", manager.index,
			ft_time(&manager));
		pid = fork();
		if (pid == 0)
		{
			// printf("IN THE CHILD PROCESS\n");
			// printf("DEBUG: Child %d starting philosophy at %ldms\n",
				manager.index, ft_time(&manager));
			return (philosophy(&manager), 0);
			// return(philosophy(&manager), 0);
		}
		else if (pid > 0)
		{
			manager.pids[manager.index] = pid;
			manager.index++;
		}
		else
			return (-1);
	}
	wait(&status);
	manager.index = 0;
	while (manager.index < manager.n_philos)
	{
		kill(manager.pids[manager.index], SIGKILL);
		manager.index++;
	}
	manager.index = 0;
	while (manager.index < manager.n_philos)
		waitpid(manager.pids[manager.index++], NULL, 0);
	sem_unlink("/philo_sem");
	return (0);
}

// printf("Correct args!\n");
// printf("philos : %d\n", manager.n_philos);
// printf("time_to_die : %d\n", manager.time_to_die);
// printf("time_to_eat : %d\n", manager.time_to_eat);
// printf("time_to_sleep : %d\n", manager.time_to_sleep);
// printf("eat_times : %d\n", manager.eat_times);