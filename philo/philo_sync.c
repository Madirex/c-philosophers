#include "philo.h"

static int	philosopher_take_forks(t_philo *philo)
{
	pthread_mutex_lock(&(philo->datacpy->forks[philo->left_fork]));
	ft_print(philo->datacpy, philo->philo_id, "has taken a fork");
	if (philo->datacpy->num_philosophers == 1)
	{
		upgrade_sleep(philo->datacpy->time_to_die);
		pthread_mutex_unlock(&(philo->datacpy->forks[philo->left_fork]));
		return (0);
	}
	pthread_mutex_lock(&(philo->datacpy->forks[philo->right_fork]));
	ft_print(philo->datacpy, philo->philo_id, "has taken a fork");
	return (1);
}

static void	philosopher_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->eating_mutex);
	ft_print(philo->datacpy, philo->philo_id, "is eating");
	philo->eating = 1;
	philo->last_meal_time = ft_current_time();
	pthread_mutex_unlock(&philo->eating_mutex);
	upgrade_sleep(philo->datacpy->time_to_eat);
	pthread_mutex_lock(&philo->eating_mutex);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->eating_mutex);
	pthread_mutex_unlock(&(philo->datacpy->forks[philo->right_fork]));
	pthread_mutex_unlock(&(philo->datacpy->forks[philo->left_fork]));
}

static void	philosopher_sleep(t_philo *philo)
{
	ft_print(philo->datacpy, philo->philo_id, "is sleeping");
	upgrade_sleep(philo->datacpy->time_to_sleep);
}

void	*philosopher_routine(void *philosopher)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)philosopher;
	i = 0;
	if ((philo->philo_id - 1) % 2 == 0)
		upgrade_sleep(philo->datacpy->time_to_eat - 10);
	pthread_mutex_lock(&philo->datacpy->stop_mutex);
	while (!philo->datacpy->stop
		&& (philo->datacpy->num_meals_required == -1
			|| i < philo->datacpy->num_meals_required))
	{
		pthread_mutex_unlock(&philo->datacpy->stop_mutex);
		ft_print(philo->datacpy, philo->philo_id, "is thinking");
		if (!philosopher_take_forks(philo))
			break ;
		philosopher_eat(philo);
		philosopher_sleep(philo);
		i++;
		pthread_mutex_lock(&philo->datacpy->stop_mutex);
	}
	if (i == philo->datacpy->num_meals_required)
		philo->finished = 1;
	pthread_mutex_unlock(&philo->datacpy->stop_mutex);
	return (NULL);
}

void	*philosopher_monitor(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *)philosopher;
	pthread_mutex_lock(&(philo->datacpy->stop_mutex));
	while (!philo->datacpy->stop && (philo->datacpy->num_meals_required == -1
			|| philo->finished == 0))
	{
		pthread_mutex_unlock(&(philo->datacpy->stop_mutex));
		pthread_mutex_lock(&philo->eating_mutex);
		if (!philo->eating && ft_current_time()
			- philo->last_meal_time >= philo->datacpy->time_to_die)
		{
			ft_print(philo->datacpy, philo->philo_id, "died");
			pthread_mutex_unlock(&philo->eating_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->eating_mutex);
		pthread_mutex_lock(&(philo->datacpy->stop_mutex));
	}
	pthread_mutex_unlock(&(philo->datacpy->stop_mutex));
	return (NULL);
}
