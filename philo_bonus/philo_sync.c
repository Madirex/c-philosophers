#include "philo.h"

static int	philosopher_take_forks(t_philo *philo)
{
	sem_wait(&(philo->datacpy->forks[philo->left_fork]));
	ft_print(philo->datacpy, philo->philo_id, "has taken a fork");
	if (philo->datacpy->num_philosophers == 1)
	{
		upgrade_sleep(philo->datacpy->time_to_die);
		sem_post(&(philo->datacpy->forks[philo->left_fork]));
		return (0);
	}
	sem_wait(&(philo->datacpy->forks[philo->right_fork]));
	ft_print(philo->datacpy, philo->philo_id, "has taken a fork");
	return (1);
}

static void	philosopher_eat(t_philo *philo)
{
	sem_wait(&philo->eating_sem);
	ft_print(philo->datacpy, philo->philo_id, "is eating");
	philo->eating = 1;
	philo->last_meal_time = ft_current_time();
	sem_post(&philo->eating_sem);
	upgrade_sleep(philo->datacpy->time_to_eat);
	sem_wait(&philo->eating_sem);
	philo->eating = 0;
	sem_post(&philo->eating_sem);
	sem_post(&(philo->datacpy->forks[philo->right_fork]));
	sem_post(&(philo->datacpy->forks[philo->left_fork]));
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
	sem_wait(&philo->datacpy->stop_sem);
	while (!philo->datacpy->stop
		&& (philo->datacpy->num_meals_required == -1
			|| i < philo->datacpy->num_meals_required))
	{
		sem_post(&philo->datacpy->stop_sem);
		ft_print(philo->datacpy, philo->philo_id, "is thinking");
		if (!philosopher_take_forks(philo))
			break ;
		philosopher_eat(philo);
		philosopher_sleep(philo);
		i++;
		sem_wait(&philo->datacpy->stop_sem);
	}
	if (i == philo->datacpy->num_meals_required)
		philo->finished = 1;
	sem_post(&philo->datacpy->stop_sem);
	return (NULL);
}

void	*philosopher_monitor(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *)philosopher;
	sem_wait(&(philo->datacpy->stop_sem));
	while (!philo->datacpy->stop && (philo->datacpy->num_meals_required == -1
			|| philo->finished == 0))
	{
		sem_post(&(philo->datacpy->stop_sem));
		sem_wait(&philo->eating_sem);
		if (!philo->eating && ft_current_time()
			- philo->last_meal_time >= philo->datacpy->time_to_die)
		{
			ft_print(philo->datacpy, philo->philo_id, "died");
			sem_post(&philo->eating_sem);
			return (NULL);
		}
		sem_post(&philo->eating_sem);
		sem_wait(&(philo->datacpy->stop_sem));
	}
	sem_post(&(philo->datacpy->stop_sem));
	return (NULL);
}
