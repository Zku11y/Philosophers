/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:44:05 by mdakni            #+#    #+#             */
/*   Updated: 2025/06/22 17:48:56 by mdakni           ###   ########.fr       */
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

int assign_manager_time(t_manager *manager, int *val)
{
    struct timeval tv;
    if(val[0] == 0 || val[1] == 0 || val[2] == 0 || val[3] == 0 || (val[4] != -1 && val[4] == 0))
    {return (-1);}
	manager->number_of_philosophers = val[0];
	manager->time_to_die = val[1];
	manager->time_to_eat = val[2];
	manager->time_to_sleep = val[3];
	manager->number_of_times_to_eat = val[4];
    manager->died = false;
    manager->all_ready = false;
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
    long check_time;
    int i;

    // printf("\e[1;31mIM HEREEEERE!!!\e[0m\n");
    manager = (t_manager *)arg;
    while(manager->all_ready == false);
    // printf("\e[1;32mmanager time since %d ate : %ld\e[0m\n", 0, manager->philos[0].time_since_ate);
    while(!manager->died)
    {
        i = 0;
        while(i < manager->number_of_philosophers)
        {
            // printf("\e[1;32mmanager time since %d ate : %ld\e[0m\n", i, manager->philos[i].time_since_ate);
            check_time = get_current_time(&manager->philos[i]) - manager->philos[i].time_since_ate;
            if(check_time > manager->time_to_die)
            {
                manager->death_time = check_time;
                manager->died = true;
                manager->death_index = i;
                return NULL;
            }
            i++;
        }
    }
    return NULL;
}

// int ft_sleep(t_philo *philo, int time_ms)
// {
//     long start_time = get_current_time(philo);
//     long elapsed;
    
//     while(1)
//     {
//         if(philo->manager->died == true)
//             return 1;
            
//         elapsed = get_current_time(philo) - start_time;
//         if(elapsed >= time_ms)
//             break;
            
//         // Sleep for smaller chunks near the end for better precision
//         long remaining = time_ms - elapsed;
//         if(remaining > 1000) // > 1ms
//             usleep(500); // Sleep 0.5ms
//         else if(remaining > 100) // > 0.1ms  
//             usleep(50);  // Sleep 0.05ms
//         else
//             usleep(10);  // Sleep 0.01ms for final precision
//     }
//     return 0;
// }

int ft_sleep(t_philo *philo, int time)
{
    long tmp_start;

    tmp_start = get_current_time(philo);
    while(get_current_time(philo) - tmp_start < time)
    {
        if(philo->manager->died == true)
            return 1;
        usleep(100);
    }
    return 0;
}

void *routine(void *arg)
{
    t_philo *philo;
    struct timeval tv;


    philo = (t_philo *)arg;
    philo->current_time = philo->manager->start_time;
    philo->time_since_ate = philo->current_time;
    while(philo->manager->philos[philo->manager->number_of_philosophers - 1].created == false);
    while(philo->manager->all_ready == false);
    gettimeofday(&tv, NULL);
    if(philo->index % 2 == 0)
    {
        // usleep(2000);
        printf("%ld %d is sleeping\n", get_current_time(philo) ,philo->index);
        if(ft_sleep(philo, philo->manager->time_to_sleep) == 1)
            return NULL;
        // usleep(philo->manager->time_to_sleep * 1000);
    }
    while(!philo->manager->died)
    {
        printf("%ld %d is thinking\n", get_current_time(philo) ,philo->index);
        pthread_mutex_lock(philo->left);
        // printf("%ld %d has taken a fork\n", get_current_time(philo) ,philo->index);
        pthread_mutex_lock(philo->right);
        // printf("%ld %d has taken a fork\n", get_current_time(philo) ,philo->index);
        philo->time_since_ate = get_current_time(philo);
        printf("%ld %d is eating\n", get_current_time(philo) ,philo->index);
        if(ft_sleep(philo, philo->manager->time_to_eat) == 1)
        {
            pthread_mutex_unlock(philo->left);
            pthread_mutex_unlock(philo->right);
            return NULL;
        }
        // usleep(philo->manager->time_to_eat * 1000);
        pthread_mutex_unlock(philo->left);
        pthread_mutex_unlock(philo->right);
        printf("%ld %d is sleeping\n", get_current_time(philo) ,philo->index);
        if(ft_sleep(philo, philo->manager->time_to_sleep) == 1)
            return NULL;
        // usleep(philo->manager->time_to_sleep * 1000);
    }
    // if(philo->manager->death_index == philo->index)
    //     printf("%ld %d died\n", get_current_time(philo) ,philo->index);
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
        manager->philos[i].right = &(manager->forks[(i + 1) % n]);
        manager->philos[i].manager = manager;
        manager->philos[i].time_since_ate = 0;
        manager->philos[i].created = false;
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
        {return(printf("\e[1;33mError : Invalid values in arguments\e[0m\n"), 1);}
	if(alloc_philo_fork(&manager) == -1)
        return(printf("\e[1;31mError : Memory allocation error\e[0m\n"), 1);
    init_philo(&manager);
    init_forks(&manager);
    manager.index = 0;
    gettimeofday(&manager.tv, NULL);
    while(manager.index < manager.number_of_philosophers)
    {

        pthread_create(&(manager.philos[manager.index].thread), NULL, &routine, &(manager.philos[manager.index]));
        manager.philos[manager.index].created = true;
        manager.index++;
    }
    pthread_create(&manager.monitor, NULL, &monitor, &manager);
    manager.start_time = (manager.tv.tv_sec * 1000) + (manager.tv.tv_usec / 1000);
    manager.all_ready = true;
    manager.index = 0;
    while(manager.index < manager.number_of_philosophers)
    {
        pthread_join(manager.philos[manager.index].thread, NULL);
        manager.index++;
    }
    pthread_join(manager.monitor, NULL);
    if(manager.died)
        printf("%ld %d died\n", manager.death_time ,manager.philos[manager.death_index].index);
    destroy_forks(&manager);
    return (0);
}

	// printf("Correct args!\n");
	// printf("philos : %d\n", manager.number_of_philosophers);
	// printf("time_to_die : %d\n", manager.time_to_die);
	// printf("time_to_eat : %d\n", manager.time_to_eat);
	// printf("time_to_sleep : %d\n", manager.time_to_sleep);
	// printf("number_of_times_to_eat : %d\n", manager.number_of_times_to_eat);