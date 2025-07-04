/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:01:04 by mel-adna          #+#    #+#             */
/*   Updated: 2025/05/24 11:10:31 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->r_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->lock);
	philo->eating = 1;
	philo->time_to_die = get_time() + philo->data->death_time;
	print_status(philo, "is eating");
	philo->eat_count++;
	if (philo->eat_count >= philo->data->meals_nb && 
		philo->data->meals_nb != -1)
		philo->is_full = 1;
	pthread_mutex_unlock(&philo->lock);
	my_usleep(philo->data->eat_time, philo);
	pthread_mutex_lock(&philo->lock);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->lock);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	int		finished;

	finished = 0;
	philo = (t_philo *)arg;
	if (philo->data->philo_num == 1)
	{
		pthread_mutex_lock(philo->l_fork);
		print_status(philo, "has taken a fork");
		my_usleep(philo->data->death_time, philo);
		return (pthread_mutex_unlock(philo->l_fork), NULL);
	}
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!philo->data->dead && !finished)
	{
		print_status(philo, "is thinking");
		eat(philo);
		print_status(philo, "is sleeping");
		my_usleep(philo->data->sleep_time, philo);
		pthread_mutex_lock(&philo->data->lock);
		finished = philo->data->finished;
		pthread_mutex_unlock(&philo->data->lock);
	}
	return (NULL);
}

void	start_simulation(t_data *data)
{
	pthread_t	monitor_t;
	int			i;

	data->start_time = get_time();
	i = 0;
	while (i < data->philo_num)
		data->philos[i++].time_to_die = data->start_time + data->death_time;
	pthread_create(&monitor_t, NULL, &monitor, data);
	i = 0;
	while (i < data->philo_num)
	{
		pthread_create(&data->philos[i].t1, NULL, &routine, &data->philos[i]);
		i++;
	}
	i = 0;
	while (i < data->philo_num)
		pthread_join(data->philos[i++].t1, NULL);
	pthread_join(monitor_t, NULL);
	cleanup(data);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
		return (printf("Error: Wrong number of arguments\n"));
	if (parse_args(argc, argv, &data))
		return (1);
	if (init_data(&data) || init_philos(&data))
		return (1);
	start_simulation(&data);
	return (0);
}
