/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 13:41:47 by mdakni            #+#    #+#             */
/*   Updated: 2025/07/06 14:20:30 by mdakni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	assign_manager_time(t_manager *manager, int ac, int *val)
{
	if (val[0] < 1 || val[0] > 200 || val[1] <= 0 || val[2] <= 0 || val[3] <= 0
		|| (val[4] != -1 && val[4] <= 0))
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
