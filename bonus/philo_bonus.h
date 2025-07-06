/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:44:07 by mdakni            #+#    #+#             */
/*   Updated: 2025/07/06 14:09:06 by mdakni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

typedef enum s_state
{
	THINKING,
	SLEEPING,
	EATING,
	DEAD
}							t_state;

typedef struct s_manager	t_manager;

typedef struct s_philo
{
	int						index;
	t_state					state;
	pthread_mutex_t			*left;
	pthread_mutex_t			*right;
	pthread_mutex_t			time_lock;
	pthread_t				thread;
	int						times_ate;
	t_manager				*manager;
	long					current_time;
	long					last_ate;
	bool					created;
}							t_philo;

typedef struct s_manager
{
	int						index;
	long					start_time;
	pthread_mutex_t			*forks;
	pthread_t				monitor;
	t_philo					*philos;
	int						n_philos;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						eat_times;
	int						times_ate;
	bool					died;
	bool					parent;
	struct timeval			tv;
	pid_t					*pids;
	sem_t					*sem;
	sem_t					*death_check;
	sem_t					*time_lock;
	sem_t					*all_ready;
	long					last_ate;
}							t_manager;

int							ft_atoi(const char *str);
void						*ft_calloc(size_t count, size_t size);
int							assign_manager_time(t_manager *manager, int ac,
								int *val);
int							check_args(t_manager *manager, int ac, char **av);
void						ft_print(t_manager *manager, char *str);
void						ft_eat(t_manager *manager);
void						ft_clean(t_manager *manager);
void						ft_fork(t_manager *manager);
void						ft_wait(t_manager *manager);
long						ft_time(t_manager *manager);
int							philosophy(t_manager *manager);