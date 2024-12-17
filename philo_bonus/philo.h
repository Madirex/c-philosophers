#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <semaphore.h>

typedef struct s_philo
{
	int				philo_id;
	pid_t			philo_pid;
	pid_t			monitor_pid;
	sem_t			eating_sem;
	struct s_data	*datacpy;
	int				eating;
	int				left_fork;
	int				right_fork;
	long long		last_meal_time;
	int				finished;
}	t_philo;

typedef struct s_data
{
	long long		initial_time;
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_meals_required;
	t_philo			*philosophers;
	int				stop;
	sem_t			*forks;
	sem_t			print_sem;
	sem_t			stop_sem;
	int				forks_sem_init;
	int				print_sem_init;
	int				stop_sem_init;
	int				philosophers_sem_init;
}	t_data;

int			ft_atoi(const char *str);
int			ft_strcmp(const char *str1, const char *str2);
void		ft_error(t_data *data, char *message);
long long	ft_current_time(void);
void		upgrade_sleep(long long time);
void		init_data(t_data *data, char **argv);
void		ft_print(t_data *data, int id, char *message);
void		*philosopher_routine(void *philosopher);
void		*philosopher_monitor(void *philosopher);
#endif