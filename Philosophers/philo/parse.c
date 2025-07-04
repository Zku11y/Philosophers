/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:01:00 by mel-adna          #+#    #+#             */
/*   Updated: 2025/05/13 12:07:23 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-')
		return (0);
	if (str[0] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	parse_args(int argc, char **argv, t_data *data)
{
	int	i;

	i = 1;
	while (i < argc)
		if (!is_numeric(argv[i++]))
			return (printf("Invalid arguments\n"), 1);
	data->philo_num = ft_atoi(argv[1]);
	data->death_time = (uint64_t)ft_atoi(argv[2]);
	data->eat_time = (uint64_t)ft_atoi(argv[3]);
	data->sleep_time = (uint64_t)ft_atoi(argv[4]);
	if (argc == 6)
		data->meals_nb = ft_atoi(argv[5]);
	else
		data->meals_nb = -1;
	if (data->philo_num < 1 || data->philo_num > 200 || data->death_time <= 0
		|| data->eat_time <= 0 || data->sleep_time <= 0 || (argc == 6
			&& data->meals_nb <= 0))
		return (printf("Invalid arguments\n"), 1);
	data->dead = 0;
	data->finished = 0;
	return (0);
}
