#include "philo.h"

static void	create_pthreads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_create(&data->philosophers[i].philo_thread, NULL,
				philosopher_routine, &data->philosophers[i]) != 0)
			ft_error(data, "Error creating thread");
		if (pthread_create(&data->philosophers[i].monitor_thread, NULL,
				philosopher_monitor, &data->philosophers[i]) != 0)
			ft_error(data, "Error creating monitor thread");
		i++;
	}
}

static void	thread_handler(t_data *data)
{
	int	i;

	create_pthreads(data);
	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_join(data->philosophers[i].philo_thread, NULL) != 0)
			ft_error(data, "Error waiting thread");
		if (pthread_join(data->philosophers[i].monitor_thread, NULL) != 0)
			ft_error(data, "Error waiting monitor thread");
		i++;
	}
	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philosophers[i].eating_mutex);
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
	thread_handler(&data);
	return (0);
}
