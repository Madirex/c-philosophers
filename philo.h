#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct s_data t_data;

typedef struct s_philo
{
    int id;
    pthread_t thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    long long last_meal_time;
    int times_ate;
    t_data *data;
} t_philo;

typedef struct s_data
{
    int num_philos;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int num_times_each_philosopher_must_eat;
    int philos_fed; // Contador de filósofos que han comido lo suficiente
    int stop_simulation; // Indicador para detener la simulación
    t_philo *philosophers;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    pthread_mutex_t meal_mutex; // Mutex para proteger el contador philos_fed
    pthread_mutex_t stop_mutex; // Mutex para proteger stop_simulation
    long long start_time;
} t_data;

void *philosopher_routine(void *arg);
void init_data(t_data *data, int argc, char **argv);
int init_philosophers(t_data *data);
void cleanup(t_data *data);
void print_status(t_philo *philo, const char *status);
void ft_usleep(long long time_in_ms); // Declaración de la función ft_usleep

#endif
