/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skully <skully@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 11:08:38 by mel-adna          #+#    #+#             */
/*   Updated: 2025/07/01 12:57:39 by skully           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

uint64_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((uint64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

void	my_usleep(uint64_t time_in_ms, t_philo *philo)
{
	uint64_t	start;

	(void)philo;
	start = get_time();
	while (get_time() - start <= time_in_ms)
		usleep(100);
}

void	print_status(t_philo *philo, char *status)
{
	t_args	*args;

	args = philo->args;
	sem_wait(args->print_lock);
	printf("%ld %d %s\n", (get_time() - args->start_time), philo->id, status);
	sem_post(args->print_lock);
}

void	cleanning(t_args *args)
{
	sem_close(args->forks);
	sem_close(args->print_lock);
	sem_close(args->stop_sim);
	sem_close(args->meal_gate);
	sem_close(args->lock);
	sem_unlink("/forks");
	sem_unlink("/print_lock");
	sem_unlink("/stop_sim");
	sem_unlink("/meal_gate");
	sem_unlink("/lock");
	free(args->pids);
}

int	ft_atoi(const char *str)
{
	int				i;
	int				sign;
	unsigned long	result;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			sign = -1;
	while (str[i] >= '0' && str[i] <= '9')
		result = result * 10 + (str[i++] - '0');
	if (result > 2147483647)
		return (-1);
	return ((int)(result * sign));
}
