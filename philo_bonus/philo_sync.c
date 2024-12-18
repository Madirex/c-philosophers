#include "philo.h"

static int	philosopher_take_forks(t_philo *philo)
{
	sem_wait(&((*philo->data)->forks[philo->left_fork]));
	ft_print(*philo->data, philo->philo_id, "has taken a fork");
	if ((*philo->data)->num_philosophers == 1)
	{
		upgrade_sleep((*philo->data)->time_to_die);
		sem_post(&((*philo->data)->forks[philo->left_fork]));
		return (0);
	}
	sem_wait(&((*philo->data)->forks[philo->right_fork]));
	ft_print(*philo->data, philo->philo_id, "has taken a fork");
	return (1);
}

static void	philosopher_eat(t_philo *philo)
{
	sem_wait(&philo->eating_sem);
	ft_print(*philo->data, philo->philo_id, "is eating");
	philo->eating = 1;
	philo->last_meal_time = ft_current_time();
	sem_post(&philo->eating_sem);
	upgrade_sleep((*philo->data)->time_to_eat);
	sem_wait(&philo->eating_sem);
	philo->eating = 0;
	sem_post(&philo->eating_sem);
	sem_post(&((*philo->data)->forks[philo->right_fork]));
	sem_post(&((*philo->data)->forks[philo->left_fork]));
}

static void	philosopher_sleep(t_philo *philo)
{
	ft_print(*philo->data, philo->philo_id, "is sleeping");
	upgrade_sleep((*philo->data)->time_to_sleep);
}

void	*philosopher_routine(void *philosopher)
{
	t_philo	*philo;
	int		i;
	sem_t *stop_sem_global;

	philo = (t_philo *)philosopher;
	stop_sem_global = sem_open("/stop_sem_global", 0);
	if (stop_sem_global == SEM_FAILED)
		ft_error(*philo->data, "Error opening stop semaphore");
	i = 0;
	if ((philo->philo_id - 1) % 2 == 0)
		upgrade_sleep((*philo->data)->time_to_eat - 10);
	sem_wait(stop_sem_global);
	while (!(*philo->data)->stop
		&& ((*philo->data)->num_meals_required == -1
			|| i < (*philo->data)->num_meals_required))
	{
		sem_post(stop_sem_global);
		ft_print(*philo->data, philo->philo_id, "is thinking");
		if (!philosopher_take_forks(philo))
			break ;
		philosopher_eat(philo);
		philosopher_sleep(philo);
		i++;
		sem_wait(stop_sem_global);
	}
	if (i == (*philo->data)->num_meals_required)
		philo->finished = 1;
	sem_post(stop_sem_global);
	return (NULL);
}

void	*philosopher_monitor(void *philosopher)
{
	t_philo	*philo;
	sem_t *stop_sem_global;

	philo = (t_philo *)philosopher;
	stop_sem_global = sem_open("/stop_sem_global", 0);
	if (stop_sem_global == SEM_FAILED)
		ft_error(*philo->data, "Error opening stop semaphore");
	sem_wait(stop_sem_global);
	while (!(*philo->data)->stop && ((*philo->data)->num_meals_required == -1
			|| philo->finished == 0))
	{
		sem_post(stop_sem_global);
		sem_wait(&philo->eating_sem);
		if (!philo->eating && ft_current_time()
			- philo->last_meal_time >= (*philo->data)->time_to_die)
		{
			ft_print(*philo->data, philo->philo_id, "died");
			sem_post(&philo->eating_sem);
			return (NULL);
		}
		sem_post(&philo->eating_sem);
		sem_wait(stop_sem_global);
	}
	sem_post(stop_sem_global);
	return (NULL);
}
