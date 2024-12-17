#include "philo.h"

static void	create_monitor_process(t_philo *philo)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		ft_error(philo->datacpy, "Error creating monitor process");
	if (pid == 0)
	{
		philosopher_monitor(philo);
		exit(0);
	}
	philo->monitor_pid = pid;
}

static void	create_processes(t_data *data)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < data->num_philosophers)
	{
		pid = fork();
		if (pid < 0)
			ft_error(data, "Error creating process");
		if (pid == 0)
		{
			philosopher_routine(&data->philosophers[i]);
			create_monitor_process(&data->philosophers[i]);
			exit(0);
		}
		data->philosophers[i].philo_pid = pid;
		i++;
	}
}

static void	process_handler(t_data *data)
{
	int		i;

	create_processes(data);
	i = 0;
	while (i < data->num_philosophers)
	{
		if (waitpid(data->philosophers[i].philo_pid, NULL, 0) < 0)
			ft_error(data, "Error waiting for philosopher process");
		if (waitpid(data->philosophers[i].monitor_pid, NULL, 0) < 0)
			ft_error(data, "Error waiting for monitor process");
		i++;
	}
	i = 0;
	while (i < data->num_philosophers)
	{
		sem_destroy(&data->forks[i]);
		sem_destroy(&data->philosophers[i].eating_sem);
		i++;
	}
	free(data->philosophers);
	free(data->forks);
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;
	int		num;

	if (argc < 5 || argc > 6)
		ft_error(NULL, \
			"Arguments invalid. Usage: ./philo " \
			"number_of_philosophers time_to_die " \
			"time_to_eat time_to_sleep " \
			"[number_of_times_each_philosopher_must_eat]");
	i = 1;
	while (argv[i])
	{
		num = ft_atoi(argv[i]);
		if (num < 0)
			ft_error(NULL, "Invalid argument. Must be a positive number");
		if (i == 1 && num < 1)
			ft_error(NULL, "At least 1 philosopher");
		i++;
	}
	init_data(&data, argv);
	process_handler(&data);
	return (0);
}
