#include "philo.h"

static void	ft_destroy_sem(t_data *data)
{
	int	i;

	if (data->print_sem_init)
		sem_destroy(&data->print_sem);
	if (data->stop_sem_init)
		sem_destroy(&data->stop_sem);
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
	sem_wait(&(data->stop_sem));
	sem_wait(&(data->print_sem));
	if (!data->stop)
	{
		if (ft_strcmp(message, "died") == 0)
			data->stop = 1;
		printf("%lld ", ft_current_time() - data->initial_time);
		printf("%d ", id + 1);
		printf("%s\n", message);
	}
	sem_post(&(data->print_sem));
	sem_post(&(data->stop_sem));
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
