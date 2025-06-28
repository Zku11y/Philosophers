/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skully <skully@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:44:05 by mdakni            #+#    #+#             */
/*   Updated: 2025/06/28 18:49:00 by skully           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long get_current_time_2(t_manager *manager)
{
    struct timeval tv;
    long current_time;

    gettimeofday(&tv, NULL);
    current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return current_time - manager->start_time;
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
            pthread_mutex_lock(&manager->time_lock);
            check_time = get_current_time_2(manager) - manager->philos[i].time_since_ate;
            pthread_mutex_unlock(&manager->time_lock);
            if(check_time > manager->time_to_die)
            {
                manager->death_time = check_time;
                pthread_mutex_lock(&manager->death_check);
                manager->died = true;
                pthread_mutex_unlock(&manager->death_check);
                manager->death_index = i;
                return NULL;
            }
            i++;
        }
    }
    return NULL;
}

int ft_sleep(t_manager *manager, long time)
{
    long tmp_start;

    tmp_start = get_current_time_2(manager);
    while(get_current_time_2(manager) - tmp_start < time)
    {
        pthread_mutex_lock(&manager->death_check);
        if(manager->died == true)
            return (pthread_mutex_unlock(&manager->death_check), 1);
        pthread_mutex_unlock(&manager->death_check);
        usleep(100);
    }
    return 0;
}
void ft_clean(t_manager *manager)
{
    pthread_mutex_destroy(&manager->death_check);
    pthread_mutex_destroy(&manager->time_lock);
    pthread_join(manager->monitor, NULL);
}

int philosophy(t_manager *manager)
{
    pthread_mutex_init(&manager->death_check, NULL);
    pthread_mutex_init(&manager->time_lock, NULL);
    pthread_create(&manager->monitor, NULL, &monitor, &manager);
    if(manager->index % 2 == 0)
    {
        printf("%ld %d is sleeping\n", get_current_time_2(manager), manager->index);
        if(ft_sleep(manager, manager->time_to_sleep) == 1)
            return(ft_clean(manager), exit(EXIT_FAILURE), 1);
    }
    while(!manager->died)
    {
        printf("%ld %d is thinking\n", get_current_time_2(manager), manager->index);
        sem_wait(manager->sem);
        sem_wait(manager->sem);
        pthread_mutex_lock(&manager->time_lock);
        manager->time_since_ate = get_current_time_2(manager);
        pthread_mutex_unlock(&manager->time_lock);
        printf("%ld %d is eating\n", get_current_time_2(manager), manager->index);
        if(ft_sleep(manager, manager->time_to_eat) == 1)
        {
            sem_post(manager->sem);
            sem_post(manager->sem);
            return(ft_clean(manager), exit(EXIT_FAILURE), 1);
        }
        sem_post(manager->sem);
        sem_post(manager->sem);
        printf("%ld %d is sleeping\n", get_current_time_2(manager), manager->index);
        if(ft_sleep(manager, manager->time_to_sleep) == 1)
            return(ft_clean(manager), exit(EXIT_FAILURE), 1);
    }
    pthread_join(manager->monitor, NULL);
    return (exit(EXIT_SUCCESS), 0);
}

int main(int ac, char **av)
{
    t_manager manager;
    pid_t pid;
    int status;
    int exit_status;

    if(ac > 6 || ac < 5)
        return(printf("\e[1;31mError : Invalid number of arguments\e[0m\n"), 1);
    if(check_args(&manager, av) == -1)
        {return(printf("\e[1;33mError : Invalid values in arguments\e[0m\n"), 1);}
    manager.pids = malloc(manager.number_of_philosophers * sizeof(pid_t) + 1);
    manager.sem = sem_open("/philo_sem", O_CREAT | O_EXCL, 0777, manager.number_of_philosophers);
    manager.index = 0;
    gettimeofday(&manager.tv, NULL);
    manager.start_time = (manager.tv.tv_sec * 1000) + (manager.tv.tv_usec / 1000);
    while(manager.index < manager.number_of_philosophers)
    {
        pid = fork();
        manager.parent = false;
        if(pid == 0)
            return(philosophy(&manager), 0);
        manager.parent = true;
        manager.pids[manager.index] = pid;
        manager.index++;
    }
    while(1)
    {
        pid = waitpid(-1, &status, 0);
        if(pid == -1)
            break;
        if(WIFEXITED(status))
        {
            exit_status = WEXITSTATUS(status);
            if(exit_status == 1)
            {
                manager.index = 0;
                while(manager.index < manager.number_of_philosophers)
                {
                    if(manager.pids[manager.index] == pid)
                        manager.death_index = manager.index;
                    else
                        kill(manager.pids[manager.death_index], SIGKILL);
                    manager.index++;
                }
                printf("%ld %d has died\n", get_current_time_2(&manager), manager.death_index);
                break;
            }
        }
    }
    while (waitpid(-1, NULL, 0) > 0);                                                                                                                    
    sem_unlink("/philo_sem");
    return (0);
}

	// printf("Correct args!\n");
	// printf("philos : %d\n", manager.number_of_philosophers);
	// printf("time_to_die : %d\n", manager.time_to_die);
	// printf("time_to_eat : %d\n", manager.time_to_eat);
	// printf("time_to_sleep : %d\n", manager.time_to_sleep);
	// printf("number_of_times_to_eat : %d\n", manager.number_of_times_to_eat);