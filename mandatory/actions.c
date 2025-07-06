/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 10:56:02 by mdakni            #+#    #+#             */
/*   Updated: 2025/07/06 11:17:20 by mdakni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_sleep(t_philo *philo, int time)
{
	long	tmp_start;

	tmp_start = ft_time(philo->manager);
	while (ft_time(philo->manager) - tmp_start < time)
	{
		pthread_mutex_lock(&philo->manager->death_check);
		if (philo->manager->died == true)
			return (pthread_mutex_unlock(&philo->manager->death_check), 1);
		pthread_mutex_unlock(&philo->manager->death_check);
		usleep(100);
	}
	return (0);
}

int	ft_print(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->manager->death_check);
	if (philo->manager->died)
		return (pthread_mutex_unlock(&philo->manager->death_check), 1);
	printf("%ld %d %s\n", ft_time(philo->manager), philo->index, str);
	pthread_mutex_unlock(&philo->manager->death_check);
	return (0);
}

int	ft_check_times_ate(t_philo *philo)
{
	pthread_mutex_lock(&philo->ate_number);
	philo->times_ate++;
	if (philo->manager->eat_times != -1
		&& philo->times_ate > philo->manager->eat_times)
	{
		pthread_mutex_lock(&philo->manager->all_philos_ate);
		philo->manager->philos_ate++;
		pthread_mutex_unlock(&philo->manager->all_philos_ate);
		return (pthread_mutex_unlock(&philo->ate_number), 1);
	}
	pthread_mutex_unlock(&philo->ate_number);
	return (0);
}

int	ft_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->left);
	if (ft_print(philo, "has taken a fork") == 1)
		return (pthread_mutex_unlock(philo->left), 1);
	pthread_mutex_lock(philo->right);
	if (ft_print(philo, "has taken a fork") == 1)
		return (pthread_mutex_unlock(philo->left),
			pthread_mutex_unlock(philo->right), 1);
	pthread_mutex_lock(&philo->time_lock);
	philo->last_ate = ft_time(philo->manager);
	pthread_mutex_unlock(&philo->time_lock);
	if (ft_print(philo, "is eating") == 1)
		return (1);
	if (ft_sleep(philo, philo->manager->time_to_eat) == 1)
	{
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		return (1);
	}
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
	if (ft_check_times_ate(philo) == 1)
		return (1);
	return (0);
}

int	ft_eat_single(t_philo *philo)
{
	pthread_mutex_lock(philo->left);
	if (ft_print(philo, "has taken a fork") == 1)
		return (1);
	while (1)
	{
		pthread_mutex_lock(&philo->manager->death_check);
		if (philo->manager->died)
			return (1);
		pthread_mutex_unlock(&philo->manager->death_check);
	}
	return (0);
}
