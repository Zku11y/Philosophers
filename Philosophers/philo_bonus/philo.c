/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdakni <mdakni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 11:08:31 by mel-adna          #+#    #+#             */
/*   Updated: 2025/07/04 17:11:07 by mdakni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	wait_children(t_args *args, pid_t *pids)
{
	int	i;
	int	status;

	wait(&status);
	i = 0;
	while (i < args->n_philo)
	{
		kill(pids[i], SIGKILL);
		i++;
	}
	i = 0;
	while (i < args->n_philo)
		waitpid(pids[i++], NULL, 0);
}

void	start_simulation(t_args *args, t_philo *philos, pid_t *pids)
{
	int	pid;

	args->start_time = get_time();
	if (args->n_philo == 1)
	{
		pid = fork();
		if (pid == 0)
		{
			philos[0].id = 1;
			philos[0].args = args;
			philos[0].last_meal = args->start_time;
			philos[0].eat_count = 0;
			print_status(&philos[0], "has taken a fork");
			my_usleep(args->t_die, &philos[0]);
			print_status(&philos[0], "died");
			cleanning(args);
			exit(0);
		}
		waitpid(pid, NULL, 0);
		exit(0);
	}
	philo_children(args, pids, philos);
	wait_children(args, pids);
	cleanning(args);
}

int	main(int argc, char **argv)
{
	t_args	args;
	t_philo	*philos;

	if (argc < 5 || argc > 6)
		return (printf("Error: Wrong number of arguments\n"));
	if (parse_args(argc, argv, &args) || init_sems(&args))
		return (1);
	philos = malloc(sizeof(t_philo) * args.n_philo);
	if (!philos)
		return (printf("Failed to allocate memory\n"));
	args.pids = malloc(sizeof(pid_t) * args.n_philo);
	if (!args.pids)
		return (free(philos), printf("Failed to allocate memory\n"));
	start_simulation(&args, philos, args.pids);
	return (0);
}
