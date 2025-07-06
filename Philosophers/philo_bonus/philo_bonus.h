/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 11:08:29 by mel-adna          #+#    #+#             */
/*   Updated: 2025/07/06 14:10:21 by mdakni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_args
{
	int			n_philo;
	int			is_full;
	int			must_eat;
	uint64_t	t_die;
	uint64_t	t_eat;
	uint64_t	t_sleep;
	uint64_t	start_time;
	sem_t		*forks;
	sem_t		*print_lock;
	sem_t		*stop_sim;
	sem_t		*meal_gate;
	sem_t		*lock;
	pid_t		*pids;
}				t_args;

typedef struct s_philo
{
	int			id;
	int			eat_count;
	long		last_meal;
	t_args		*args;
}				t_philo;

int				parse_args(int argc, char **argv, t_args *args);
int				ft_atoi(const char *str);
uint64_t		get_time(void);
void			my_usleep(uint64_t time_in_ms, t_philo *philo);
int				init_sems(t_args *args);
void			print_status(t_philo *philo, char *status);
void			cleanning(t_args *args);
void			philo_children(t_args *args, pid_t *pids, t_philo *philos);
#endif