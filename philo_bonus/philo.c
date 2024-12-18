#include "philo.h"

static void create_processes(t_data *data)
{
    int     		i;
    pid_t   		pid;

    i = 0;
    while (i < data->num_philosophers)
    {
        pid = fork();
        if (pid == -1)
            ft_error(data, "Error creating process");
        if (pid == 0)
        {
            if (pthread_create(&data->philosophers[i].monitor_thread, NULL,
                philosopher_monitor, &data->philosophers[i]) != 0)
                ft_error(data, "Error creating monitor thread");
            pthread_detach(data->philosophers[i].monitor_thread);
            philosopher_routine(&data->philosophers[i]);
            exit(0);
        }
        else
            data->philosophers[i].philo_pid = pid;
        i++;
    }
}

static void process_handler(t_data *data)
{
    int i;

    create_processes(data);

    i = 0;
    while (i < data->num_philosophers)
    {
        if (waitpid(data->philosophers[i].philo_pid, NULL, 0) == -1)
            ft_error(data, "Error waiting for philosopher process");
        i++;
    }
    i = 0;
    while (i < data->num_philosophers)
    {
        sem_post(&data->forks[i]);
        sem_post(&data->philosophers[i].eating_sem);
        i++;
    }
    free(data->philosophers);
    free(data->forks);
}

//FIXME: programa no termina cuando un filósofo muere.
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
