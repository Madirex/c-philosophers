#include "philo.h"

static void	init_semaphores(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	if (sem_init(&data->stop_sem, 0, 1) != 0)
		ft_error(data, "Error initializing stop semaphore");
	data->stop_sem_init = 1;
	if (sem_init(&data->print_sem, 0, 1) != 0)
		ft_error(data, "Error initializing print semaphore");
	data->print_sem_init = 1;
	while (i < data->num_philosophers)
	{
		if (sem_init(&data->forks[i], 0, 1) != 0)
		{
			j = 0;
			while (j < i)
				sem_destroy(&data->forks[j++]);
			ft_error(data, "Error initializing forks semaphore");
		}
		i++;
	}
	data->forks_sem_init = 1;
}

static void	init_philosophers(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < data->num_philosophers)
	{
		if (sem_init(&data->philosophers[i].eating_sem, 0, 1) != 0)
		{
			while (j < i)
				sem_destroy(&data->philosophers[j++].eating_sem);
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
	data->philosophers_sem_init = 1;
}

void	init_data(t_data *data, char **argv)
{
	data->print_sem_init = 0;
	data->stop_sem_init = 0;
	data->forks_sem_init = 0;
	data->philosophers_sem_init = 0;
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
	data->forks = (sem_t *)malloc(sizeof(sem_t) * data->num_philosophers);
	if (!data->forks)
		ft_error(data, "Allocating memory error with forks");
	init_semaphores(data);
	init_philosophers(data);
}
