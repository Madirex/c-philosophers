#include "philo.h"

static void	init_mutex(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
		ft_error(data, "Error initializing stop mutex");
	data->stop_mutex_init = 1;
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		ft_error(data, "Error initializing print mutex");
	data->print_mutex_init = 1;
	while (i < data->num_philosophers)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			j = 0;
			while (j < i)
				pthread_mutex_destroy(&data->forks[j++]);
			ft_error(data, "Error initializing forks mutex");
		}
		i++;
	}
	data->forks_mutex_init = 1;
}

static void	init_philosophers(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_mutex_init(&data->philosophers[i].eating_mutex, NULL) != 0)
		{
			while (j < i)
				pthread_mutex_destroy(&data->philosophers[j++].eating_mutex);
			ft_error(data, "Error initializing eating mutex");
		}
		data->philosophers[i].finished = 0;
		data->philosophers[i].philo_id = i;
		data->philosophers[i].datacpy = data;
		data->philosophers[i].last_meal_time = ft_current_time();
		data->philosophers[i].eating = 0;
		data->philosophers[i].left_fork = i;
		data->philosophers[i].right_fork = (i + 1) % data->num_philosophers;
		i++;
	}
	data->philosophers_mutex_init = 1;
}

void	init_data(t_data *data, char **argv)
{
	data->print_mutex_init = 0;
	data->stop_mutex_init = 0;
	data->forks_mutex_init = 0;
	data->philosophers_mutex_init = 0;
	data->num_philosophers = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (data->time_to_eat < 3 || data->time_to_sleep < 3)
		ft_error(data, "Time to eat and time to sleep must be at least 3ms");
	data->num_meals_required = -1;
	if (argv[5])
		data->num_meals_required = ft_atoi(argv[5]);
	data->initial_time = ft_current_time();
	data->stop = 0;
	data->philosophers = (t_philo *)malloc(sizeof(t_philo)
			* data->num_philosophers);
	if (!data->philosophers)
		ft_error(data, "Allocating memory error with philosophers");
	data->forks = (pthread_mutex_t *)
		malloc(sizeof(pthread_mutex_t) * data->num_philosophers);
	if (!data->forks)
		ft_error(data, "Allocating memory error with forks");
	init_mutex(data);
	init_philosophers(data);
}
