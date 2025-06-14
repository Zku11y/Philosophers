/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:44:05 by mdakni            #+#    #+#             */
/*   Updated: 2025/06/14 12:25:36 by mdakni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int assign_manager_time(t_manager *manager, int *val)
{
    struct timeval tv;
    if(val[0] == 0 || val[1] == 0 || val[2] == 0 || val[3] == 0 || (val[4] != -1 && val[4] == 0))
        return (-1);
	manager->number_of_philosophers = val[0];
	manager->time_to_die = val[1];
	manager->time_to_eat = val[2];
	manager->time_to_sleep = val[3];
	manager->number_of_times_to_eat = val[4];
    manager->died = false;
    gettimeofday(&tv, NULL);
    manager->start_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (0);
}

int check_args(t_manager *manager, char **av)
{
    int i;
    int val[6];

    i = 1;
    while(av[i])
    {
        val[i - 1] = ft_atoi(av[i]);
        i++;
    }
    val[i - 1] = -1;
	if(assign_manager_time(manager, val) == -1)
		return -1;
	return 0;
}

int alloc_philo_fork(t_manager *manager)
{
    manager->philos = ft_calloc(manager->number_of_philosophers + 1, sizeof(t_philo));
    if(manager->philos == NULL)
        return -1;
    manager->forks = ft_calloc(manager->number_of_philosophers + 1, sizeof(pthread_mutex_t));
    if(manager->forks == NULL)
        return (free(manager->philos), -1);
    return 0;
}

void *monitor(void *arg)
{
    t_manager *manager;
    int i;

    manager = (t_manager *)arg;
    while(!manager->died)
    {
        i = 0;
        while(i < manager->number_of_philosophers)
        {
            if(manager->philos[i].state == DEAD)
            {
                manager->died = true;
                break;
            }
            i++;
        }
    }
    return NULL;
}

long get_current_time(t_philo *philo)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    philo->current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return philo->current_time - philo->manager->start_time;
}

void *routine(void *arg)
{
    t_philo *philo;
    struct timeval tv;


    philo = (t_philo *)arg;
    gettimeofday(&tv, NULL);
    philo->current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    if(philo->index % 2 == 0)
        usleep(1000);
    while(!philo->manager->died)
    {
        printf("%ld %d is thinking\n", get_current_time(philo) ,philo->index);
        pthread_mutex_lock(philo->left);
        printf("%ld %d has taken a fork\n", get_current_time(philo) ,philo->index);
        pthread_mutex_lock(philo->right);
        printf("%ld %d has taken a fork\n", get_current_time(philo) ,philo->index);
        printf("%ld %d is eating\n", get_current_time(philo) ,philo->index);
        usleep(philo->manager->time_to_eat * 1000);
        pthread_mutex_unlock(philo->left);
        pthread_mutex_unlock(philo->right);
        printf("%ld %d is sleeping\n", get_current_time(philo) ,philo->index);
        usleep(philo->manager->time_to_sleep * 1000);
    }
    return NULL;
}

void init_philo(t_manager *manager)
{
    int i;
    int n;

    i = 0;
    n = manager->number_of_philosophers;
    while(i < n)
    {
        manager->philos[i].index = i;
        manager->philos[i].left = &(manager->forks[(i - 1 + n) % n]);
        manager->philos[i].right = &(manager->forks[(i + 1) % n]);
        manager->philos[i].manager = manager;
        i++;
    }

}

void init_forks(t_manager *manager)
{
    int i;

    i = 0;
    while(i < manager->number_of_philosophers)
    {
        pthread_mutex_init(&(manager->forks[i]), NULL);
        i++;
    }
}

void destroy_forks(t_manager *manager)
{
    int i;

    i = 0;
    while(i < manager->number_of_philosophers)
    {
        pthread_mutex_destroy(&(manager->forks[i]));
        i++;
    }
}

int main(int ac, char **av)
{
    t_manager manager;

    if(ac > 6 || ac < 5)
        return(printf("\e[1;31mError : Invalid number of arguments\e[0m\n"), 1);
    if(check_args(&manager, av) == -1)
        return(printf("\e[1;33mError : Invalid values in arguments\e[0m\n"), 1);
	if(alloc_philo_fork(&manager) == -1)
        return(printf("\e[1;31mError : Memory allocation error\e[0m\n"), 1);
    init_philo(&manager);
    init_forks(&manager);
    manager.index = 0;
    while(manager.index < manager.number_of_philosophers)
    {
        pthread_create(&(manager.philos[manager.index].thread), NULL, &routine, &(manager.philos[manager.index]));
        manager.index++;
    }
    manager.index = 0;
    while(manager.index < manager.number_of_philosophers)
    {
        pthread_join(manager.philos[manager.index].thread, NULL);
        manager.index++;
    }
    destroy_forks(&manager);
    return (0);
    // pthread_create(manager.monitor, NULL, &monitor, NULL);
}
	// printf("Correct args!\n");
	// printf("philos : %d\n", manager.number_of_philosophers);
	// printf("time_to_die : %d\n", manager.time_to_die);
	// printf("time_to_eat : %d\n", manager.time_to_eat);
	// printf("time_to_sleep : %d\n", manager.time_to_sleep);
	// printf("number_of_times_to_eat : %d\n", manager.number_of_times_to_eat);