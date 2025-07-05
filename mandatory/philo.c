/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:44:05 by mdakni            #+#    #+#             */
/*   Updated: 2025/07/05 21:27:30 by mdakni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long get_current_time(t_philo *philo)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    philo->current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return philo->current_time - philo->manager->start_time;
}

long get_current_time_2(t_manager *manager)
{
    struct timeval tv;
    long current_time;

    gettimeofday(&tv, NULL);
    current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return current_time - manager->start_time;
}

int assign_manager_time(t_manager *manager, int ac, int *val)
{
    if(val[0] < 1 || val[0] > 200 || val[1] <= 0 || val[2] <= 0 || val[3] <= 0 || (val[4] != -1 && val[4] <= 0))
        return (-1);
	manager->number_of_philosophers = val[0];
	manager->time_to_die = val[1];
	manager->time_to_eat = val[2];
	manager->time_to_sleep = val[3];
    if(ac == 5)
    	manager->number_of_times_to_eat = -1;
    else
    	manager->number_of_times_to_eat = val[4];
    manager->died = false;
    manager->all_ready = false;
    manager->philos_ate = 0;
    pthread_mutex_init(&manager->death_check, NULL);
    pthread_mutex_init(&manager->all_philos_eat, NULL);
	return (0);
}

int check_args(t_manager *manager, int ac, char **av)
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
	if(assign_manager_time(manager, ac, val) == -1)
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
    long check_time;
    int i;

    manager = (t_manager *)arg;
    while(manager->all_ready == false);
    while(1)
    {
        i = 0;
        while(i < manager->number_of_philosophers)
        {
            pthread_mutex_lock(&manager->philos[i].time_lock);
            check_time = get_current_time_2(manager) - manager->philos[i].time_since_ate;
            pthread_mutex_unlock(&manager->philos[i].time_lock);
            if(check_time > manager->time_to_die)
            {
                manager->death_time = check_time;
                pthread_mutex_lock(&manager->death_check);
                manager->died = true;
                printf("%ld %d died\n", get_current_time_2(manager) ,i);
                // if(manager->number_of_philosophers == 1)
                pthread_mutex_unlock(&manager->death_check);
                manager->death_index = i;
                return NULL;
            }
            pthread_mutex_lock(&manager->all_philos_eat);
            if(manager->number_of_times_to_eat != -1 && manager->philos_ate == manager->number_of_philosophers)
                return (pthread_mutex_unlock(&manager->philos[i].ate_number), NULL);
            pthread_mutex_unlock(&manager->all_philos_eat);
            i++;
        }
        // usleep(100);
    }
    return NULL;
}

int ft_sleep(t_philo *philo, int time)
{
    long tmp_start;

    tmp_start = get_current_time(philo);
    while(get_current_time(philo) - tmp_start < time)
    {
        pthread_mutex_lock(&philo->manager->death_check);
        if(philo->manager->died == true)
            return (pthread_mutex_unlock(&philo->manager->death_check), 1);
        pthread_mutex_unlock(&philo->manager->death_check);
        usleep(100);
    }
    return (0);
}

int ft_print(t_philo *philo, char *str)
{
    pthread_mutex_lock(&philo->manager->death_check);
    if(philo->manager->died)
        return (pthread_mutex_unlock(&philo->manager->death_check), 1);
    printf("%ld %d %s\n", get_current_time_2(philo->manager) ,philo->index, str);
    pthread_mutex_unlock(&philo->manager->death_check);
        return (0);
}

int ft_eat(t_philo *philo)
{
    pthread_mutex_lock(philo->left);
    if(ft_print(philo, "has taken a fork") == 1)
        return (pthread_mutex_unlock(philo->left), 1);
    pthread_mutex_lock(philo->right);
    if(ft_print(philo, "has taken a fork") == 1)
        return (pthread_mutex_unlock(philo->left), pthread_mutex_unlock(philo->right), 1);
    pthread_mutex_lock(&philo->time_lock);
    philo->time_since_ate = get_current_time_2(philo->manager);
    pthread_mutex_unlock(&philo->time_lock);
    if(ft_print(philo, "is eating") == 1)
        return 1;
    if(ft_sleep(philo, philo->manager->time_to_eat) == 1)
    {
        pthread_mutex_unlock(philo->left);
        pthread_mutex_unlock(philo->right);
        return 1;
    }
    pthread_mutex_unlock(philo->left);
    pthread_mutex_unlock(philo->right);
    pthread_mutex_lock(&philo->ate_number);
    philo->times_ate++;
    if(philo->manager->number_of_times_to_eat != -1 && philo->times_ate > philo->manager->number_of_times_to_eat)
    {
        pthread_mutex_lock(&philo->manager->all_philos_eat);
        philo->manager->philos_ate++;
        pthread_mutex_unlock(&philo->manager->all_philos_eat);
        return (pthread_mutex_unlock(&philo->ate_number), 1);
    }
    pthread_mutex_unlock(&philo->ate_number);
    return 0;
}

int ft_eat_single(t_philo *philo)
{
    pthread_mutex_lock(philo->left);
    if(ft_print(philo, "has taken a fork") == 1)
        return 1;
    while(1)
    {
        pthread_mutex_lock(&philo->manager->death_check);
        if(philo->manager->died)
            return 1;
        pthread_mutex_unlock(&philo->manager->death_check);
    }
    return 0;
}

void *routine(void *arg)
{
    t_philo *philo;
    struct timeval tv;

    philo = (t_philo *)arg;
    while(philo->manager->all_ready == false);
    gettimeofday(&tv, NULL);
    if(philo->index % 2 == 0)
    {
        printf("%ld %d is sleeping\n", get_current_time_2(philo->manager) ,philo->index);
        if(ft_sleep(philo, philo->manager->time_to_sleep) == 1)
            return NULL;
    }
    while(1)
    {
        printf("%ld %d is thinking\n", get_current_time_2(philo->manager) ,philo->index);
        if(philo->manager->number_of_philosophers == 1 && ft_eat_single(philo) == 1)
            return NULL;
        else if(philo->manager->number_of_philosophers != 1 && ft_eat(philo) == 1)
            return NULL;
        printf("%ld %d is sleeping\n", get_current_time_2(philo->manager) ,philo->index);
        if(ft_sleep(philo, philo->manager->time_to_sleep) == 1)
            return NULL;
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
        manager->philos[i].left = &(manager->forks[i]);
        if(manager->number_of_philosophers == 1)
            manager->philos[i].right = NULL;
        else
            manager->philos[i].right = &(manager->forks[(i + 1) % n]);
        manager->philos[i].manager = manager;
        manager->philos[i].time_since_ate = get_current_time(&manager->philos[i]);
        manager->philos[i].created = false;
        manager->philos[i].times_ate = 0;
        pthread_mutex_init(&(manager->philos[i].time_lock), NULL);
        pthread_mutex_init(&(manager->philos[i].ate_number), NULL);
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

void destroy_mutex(t_manager *manager)
{
    int i;

    i = 0;
    while(i < manager->number_of_philosophers)
    {
        pthread_mutex_destroy(&(manager->forks[i]));
        pthread_mutex_destroy(&(manager->philos[i].time_lock));
        pthread_mutex_destroy(&(manager->philos[i].ate_number));
        i++;
    }
    pthread_mutex_destroy(&manager->death_check);
    pthread_mutex_destroy(&manager->all_philos_eat);
}

int main(int ac, char **av)
{
    t_manager manager;

    if(ac > 6 || ac < 5)
        return(printf("\e[1;31mError : Invalid number of arguments\e[0m\n"), 1);
    if(check_args(&manager, ac, av) == -1)
        {return(printf("\e[1;33mError : Invalid values in arguments\e[0m\n"), 1);}
	if(alloc_philo_fork(&manager) == -1)
        return(printf("\e[1;31mError : Memory allocation error\e[0m\n"), 1);
    gettimeofday(&manager.tv, NULL);
    manager.start_time = (manager.tv.tv_sec * 1000) + (manager.tv.tv_usec / 1000);
    init_philo(&manager);
    init_forks(&manager);
    manager.index = 0;
    while(manager.index < manager.number_of_philosophers)
    {

        pthread_create(&(manager.philos[manager.index].thread), NULL, &routine, &(manager.philos[manager.index]));
        manager.index++;
    }
    pthread_create(&manager.monitor, NULL, &monitor, &manager);
    manager.all_ready = true;
    manager.index = 0;
    while(manager.index < manager.number_of_philosophers)
    {
        pthread_join(manager.philos[manager.index].thread, NULL);
        manager.index++;
    }
    pthread_join(manager.monitor, NULL);        
    destroy_mutex(&manager);
    return (0);
}
