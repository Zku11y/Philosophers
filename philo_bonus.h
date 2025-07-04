/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:44:07 by mdakni            #+#    #+#             */
/*   Updated: 2025/07/04 20:32:55 by mdakni           ###   ########.fr       */
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
#include <fcntl.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <signal.h>

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
    pthread_t monitor;
    t_philo *philos;
    int number_of_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_times_to_eat;
    int times_ate;
    bool died;
    bool parent;
    struct timeval tv;
    pid_t *pids;
    sem_t *sem;
    sem_t *death_check;
    sem_t *time_lock;
    sem_t *all_ready;
    long time_since_ate;
}   t_manager;


int	ft_atoi(const char *str);
void	*ft_calloc(size_t count, size_t size);