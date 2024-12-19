#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_philo
{
	int				philo_id;
	pthread_t		philo_thread;
	pthread_mutex_t	eating_mutex;
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
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	int				forks_mutex_init;
	int				print_mutex_init;
	int				stop_mutex_init;
	int				philosophers_mutex_init;
}	t_data;

int			ft_atoi(const char *str);
int			ft_strcmp(const char *str1, const char *str2);
void		ft_error(t_data *data, char *message);
long long	ft_current_time(void);
void		upgrade_sleep(long long time);
void		init_data(t_data *data, char **argv);
void		ft_print(t_data *data, int id, char *message);
void		*philosopher_routine(void *philosopher);
#endif