/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:44:07 by mdakni            #+#    #+#             */
/*   Updated: 2025/07/05 17:59:37 by mdakni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdbool.h>

typedef enum s_state
{
    THINKING,
    SLEEPING,
    EATING,
    DEAD
}   t_state;

typedef struct s_manager t_manager;

typedef struct s_philo
{
    int index;
    t_state state;
    pthread_mutex_t *left;
    pthread_mutex_t *right;
    pthread_mutex_t time_lock;
    pthread_mutex_t ate_number;
    pthread_t thread;
    int times_ate;
    t_manager *manager;
    long current_time;
    long time_since_ate;
    bool created;
}   t_philo;

typedef struct s_manager
{
    int index;
    int death_index;
    long death_time;
    long start_time;
    pthread_mutex_t *forks;
    pthread_mutex_t death_check;
    pthread_mutex_t all_philos_eat;
    pthread_t monitor;
    t_philo *philos;
    int number_of_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_times_to_eat;
    bool died;
    bool all_ready;
    struct timeval tv;
    int philos_ate;
}   t_manager;


int	ft_atoi(const char *str);
void	*ft_calloc(size_t count, size_t size);
void	*ft_memset(void *b, int c, size_t len);