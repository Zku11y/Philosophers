/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:44:07 by mdakni            #+#    #+#             */
/*   Updated: 2025/07/06 11:17:47 by mdakni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_manager	t_manager;

typedef struct s_philo
{
	int						index;
	pthread_mutex_t			*left;
	pthread_mutex_t			*right;
	pthread_mutex_t			time_lock;
	pthread_mutex_t			ate_number;
	pthread_t				thread;
	int						times_ate;
	t_manager				*manager;
	long					last_ate;
}							t_philo;

typedef struct s_manager
{
	long					start_time;
	pthread_mutex_t			*forks;
	pthread_mutex_t			death_check;
	pthread_mutex_t			all_philos_ate;
	pthread_t				monitor;
	t_philo					*philos;
	int						n_philos;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						eat_times;
	bool					died;
	bool					all_ready;
	struct timeval			tv;
	int						philos_ate;
}							t_manager;

int							ft_atoi(const char *str);
void						*ft_calloc(size_t count, size_t size);
void						*ft_memset(void *b, int c, size_t len);
long						ft_time(t_manager *manager);
int							ft_sleep(t_philo *philo, int time);
int							ft_print(t_philo *philo, char *str);
int							ft_check_times_ate(t_philo *philo);
int							ft_eat(t_philo *philo);
int							ft_eat_single(t_philo *philo);
int							alloc_philo_fork(t_manager *manager);
int							assign_manager_time(t_manager *manager, int ac,
								int *val);
int							check_args(t_manager *manager, int ac, char **av);
int							ft_init(t_manager *manager, int ac, char **av);
void						init_philo(t_manager *manager);
void						init_forks(t_manager *manager);