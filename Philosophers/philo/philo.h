/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-adna <mel-adna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:01:09 by mel-adna          #+#    #+#             */
/*   Updated: 2025/05/13 18:10:58 by mel-adna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

struct	s_data;

typedef struct s_philo
{
	struct s_data	*data;
	pthread_t		t1;
	int				id;
	int				eat_count;
	int				status;
	int				is_full;
	int				eating;
	uint64_t		time_to_die;
	pthread_mutex_t	lock;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
}					t_philo;

typedef struct s_data
{
	pthread_t		*tid;
	int				philo_num;
	int				meals_nb;
	int				dead;
	int				finished;
	t_philo			*philos;
	uint64_t		death_time;
	uint64_t		eat_time;
	uint64_t		sleep_time;
	uint64_t		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	die;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;
}					t_data;

int					ft_atoi(const char *str);
int					parse_args(int argc, char **argv, t_data *data);
void				my_usleep(uint64_t time_in_ms, t_philo *philo);
void				print_status(t_philo *philo, char *msg);
uint64_t			get_time(void);
int					init_philos(t_data *data);
int					init_data(t_data *data);
void				*monitor(void *arg);
void				cleanup(t_data *data);
void				start_simulation(t_data *data);
void				eat(t_philo *philo);
void				*routine(void *arg);
#endif
