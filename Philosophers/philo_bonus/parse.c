/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 11:08:25 by mel-adna          #+#    #+#             */
/*   Updated: 2025/07/04 17:35:23 by mdakni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_sems(t_args *args)
{
	sem_unlink("/forks");
	sem_unlink("/print_lock");
	sem_unlink("/stop_sim");
	sem_unlink("/meal_gate");
	sem_unlink("/lock");
	args->forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, args->n_philo);
	args->print_lock = sem_open("/print_lock", O_CREAT | O_EXCL, 0644, 1);
	args->stop_sim = sem_open("/stop_sim", O_CREAT | O_EXCL, 0644, 0);
	args->meal_gate = sem_open("/meal_gate", O_CREAT | O_EXCL, 0644, 0);
	args->lock = sem_open("/lock", O_CREAT | O_EXCL, 0644, 1);
	if (args->lock == SEM_FAILED || args->forks == SEM_FAILED
		|| args->print_lock == SEM_FAILED || args->stop_sim == SEM_FAILED
		|| args->meal_gate == SEM_FAILED)
		return (printf("Error: Failed to create lock semaphore\n"), 1);
	return (0);
}

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
		return (0);
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	parse_args(int argc, char **argv, t_args *args)
{
	int	i;

	i = 1;
	while (i < argc)
		if (!is_numeric(argv[i++]))
			return (printf("Invalid arguments\n"), 1);
	args->n_philo = ft_atoi(argv[1]);
	args->t_die = ft_atoi(argv[2]);
	args->t_eat = ft_atoi(argv[3]);
	args->t_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		args->must_eat = ft_atoi(argv[5]);
	else
		args->must_eat = -1;
	if (args->n_philo < 1 || args->n_philo > 200 || args->t_die <= 0
		|| args->t_eat <= 0 || args->t_sleep <= 0 || (argc == 6
			&& args->must_eat <= 0))
		return (printf("Invalid arguments\n"), 1);
	return (0);
}
