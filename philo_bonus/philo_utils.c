#include "philo.h"

void	ft_destroy_sem(t_data *data)
{
	int	i;

	if (data->stop_sem_init)
		sem_unlink("/stop_sem_global");
	if (data->print_sem_init)
		sem_unlink("/print_sem_global");
	if (data->forks_sem_init)
	{
		i = 0;
		while (i < data->num_philosophers)
			sem_destroy(&data->forks[i++]);
	}
	if (data->philosophers_sem_init)
	{
		i = 0;
		while (i < data->num_philosophers)
			sem_destroy(&data->philosophers[i++].eating_sem);
	}
}

void	ft_error(t_data *data, char *message)
{
	if (data)
	{
		if (data->philosophers)
			free(data->philosophers);
		if (data->forks)
			free(data->forks);
		ft_destroy_sem(data);
	}
	printf("Error:\n%s\n", message);
	exit(1);
}

void	ft_print(t_data *data, int id, char *message)
{
	sem_t *stop_sem_global;
	sem_t *print_sem_global;

	stop_sem_global = sem_open("/stop_sem_global", 0);
	if (stop_sem_global == SEM_FAILED)
		ft_error(data, "Error opening stop semaphore");
	print_sem_global = sem_open("/print_sem_global", 0);
	if (print_sem_global == SEM_FAILED)
		ft_error(data, "Error opening print semaphore");
	sem_wait(stop_sem_global);
	sem_wait(print_sem_global);
	if (!data->stop)
	{
		if (ft_strcmp(message, "died") == 0)
			data->stop = 1;
		printf("%lld ", ft_current_time() - data->initial_time);
		printf("%d ", id + 1);
		printf("%s\n", message);
	}
	sem_post(print_sem_global);
	sem_post(stop_sem_global);
}

long long	ft_current_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	upgrade_sleep(long long time)
{
	struct timeval	start;
	struct timeval	current;
	long long		elapsed_time;

	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&current, NULL);
		elapsed_time = (current.tv_sec - start.tv_sec) * 1000000
			+ (current.tv_usec - start.tv_usec);
		if (elapsed_time >= time * 1000)
			break ;
		usleep(200);
	}
}
