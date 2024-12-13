#include "philo.h"

long long get_time_in_ms()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}

int check_death(t_philo *philo, t_data *data)
{
    if (data->stop_simulation)
        return 1;  // La simulación ya se detuvo, no es necesario imprimir que el filósofo ha muerto
    if (get_time_in_ms() - philo->last_meal_time > data->time_to_die)
    {
        pthread_mutex_lock(&data->stop_mutex);
        if (data->stop_simulation)
        {
            pthread_mutex_unlock(&data->stop_mutex);
            return 1;
        }
        data->stop_simulation = 1;
        pthread_mutex_unlock(&data->stop_mutex);
        print_status(philo, "died");
        return 1;  // El filósofo ha muerto
    }
    return 0;  // El filósofo sigue vivo
}
void *philosopher_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_data *data = philo->data;

    if (data->num_philos == 1) //TODO: ¿esto es correcto? ¿es lo que se pide, o debería quedarse en loop infinito?
    {
        print_status(philo, "is thinking");
        //ft_usleep(data->time_to_die);  // Espera el tiempo de morir y luego termina
        print_status(philo, "died");
        return NULL;
    }

    while (1)
    {
        int philosopher_satisfied = 0;

        if (data->num_times_each_philosopher_must_eat == -1)
            philosopher_satisfied = 0;
        else if (philo->times_ate == data->num_times_each_philosopher_must_eat)
            philosopher_satisfied = 1;

        if (!philosopher_satisfied)
        {
            // Pensando
            print_status(philo, "is thinking");
            if (check_death(philo, data))  // Verificar muerte al principio de cada ciclo
                break;

            // Cambiar el orden de adquisición de tenedores
            if (philo->id % 2 == 0) {
                pthread_mutex_lock(philo->right_fork);
                if (check_death(philo, data))
                {
                    pthread_mutex_unlock(philo->right_fork);
                    break;
                }
                print_status(philo, "has taken a fork");
                pthread_mutex_lock(philo->left_fork);
                if (check_death(philo, data))
                {
                    pthread_mutex_unlock(philo->right_fork);
                    pthread_mutex_unlock(philo->left_fork);
                    break;
                }
                print_status(philo, "has taken a fork");
            } else {
                pthread_mutex_lock(philo->left_fork);
                if (check_death(philo, data))
                {
                    pthread_mutex_unlock(philo->left_fork);
                    break;
                }
                print_status(philo, "has taken a fork");
                pthread_mutex_lock(philo->right_fork);
                if (check_death(philo, data))
                {
                    pthread_mutex_unlock(philo->right_fork);
                    pthread_mutex_unlock(philo->left_fork);
                    break;
                }
                print_status(philo, "has taken a fork");
            }
            // Comer
            print_status(philo, "is eating");
            usleep(data->time_to_eat * 1000);
            printf("%lld\n", get_time_in_ms() - philo->data->start_time);
            philo->last_meal_time = get_time_in_ms();
            philo->times_ate++;
            pthread_mutex_lock(&data->meal_mutex);
            if (philo->times_ate == data->num_times_each_philosopher_must_eat)
                data->philos_fed++;
            pthread_mutex_unlock(&data->meal_mutex);
            pthread_mutex_unlock(philo->right_fork);
            pthread_mutex_unlock(philo->left_fork);

            if (check_death(philo, data))  // Verificar muerte después de comer
                break;
            // Dormir
            print_status(philo, "is sleeping");
            usleep(data->time_to_sleep * 1000);
            if (check_death(philo, data))  // Verificar muerte después de dormir
                break;
        }

        // Comprobar si todos los filósofos han comido lo suficiente
        if (data->num_times_each_philosopher_must_eat != -1)
        {
            pthread_mutex_lock(&data->meal_mutex);
            if (data->philos_fed == data->num_philos)
            {
                data->stop_simulation = 1;  // Detener la simulación después de que todos coman
                pthread_mutex_unlock(&data->meal_mutex);
                break;
            }
            pthread_mutex_unlock(&data->meal_mutex);

            // Si la simulación debe parar, lo hacemos
            if (data->stop_simulation)
                break;
        }
    }
    return NULL;
}



void init_data(t_data *data, int argc, char **argv)
{
    data->num_philos = atoi(argv[1]);
    data->time_to_die = atoi(argv[2]);
    data->time_to_eat = atoi(argv[3]);
    data->time_to_sleep = atoi(argv[4]);
    data->num_times_each_philosopher_must_eat = argc == 6 ? atoi(argv[5]) : -1;
    data->philosophers = malloc(sizeof(t_philo) * data->num_philos);
    data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
    data->philos_fed = 0;
    data->stop_simulation = 0;
    for (int i = 0; i < data->num_philos; i++) {
        pthread_mutex_init(&data->forks[i], NULL);
    }
    pthread_mutex_init(&data->print_mutex, NULL);
    pthread_mutex_init(&data->meal_mutex, NULL);
    pthread_mutex_init(&data->stop_mutex, NULL);
}

int init_philosophers(t_data *data)
{
    for (int i = 0; i < data->num_philos; i++)
    {
        data->philosophers[i].id = i + 1;
        data->philosophers[i].left_fork = &data->forks[i];
        data->philosophers[i].right_fork = &data->forks[(i + 1) % data->num_philos];
        data->philosophers[i].times_ate = 0;
        data->philosophers[i].last_meal_time = get_time_in_ms();
        data->philosophers[i].data = data;
        if (pthread_create(&data->philosophers[i].thread, NULL, philosopher_routine, &data->philosophers[i]) != 0)
            return (1);
    }
    return 0;
}

void cleanup(t_data *data)
{
    for (int i = 0; i < data->num_philos; i++)
        pthread_mutex_destroy(&data->forks[i]);
    pthread_mutex_destroy(&data->print_mutex);
    pthread_mutex_destroy(&data->meal_mutex);
    pthread_mutex_destroy(&data->stop_mutex);
    free(data->philosophers);
    free(data->forks);
}

void print_status(t_philo *philo, const char *status)
{
    long long time = get_time_in_ms() - philo->data->start_time;
    pthread_mutex_lock(&philo->data->print_mutex);
    pthread_mutex_lock(&philo->data->stop_mutex);
    printf("%lld %d %s\n", time, philo->id, status);
    pthread_mutex_unlock(&philo->data->stop_mutex);
    pthread_mutex_unlock(&philo->data->print_mutex);
}

int main(int argc, char **argv)
{
    t_data data;

    if (argc < 5 || argc > 6)
    {
        printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        return (1);
    }
    init_data(&data, argc, argv);
    data.start_time = get_time_in_ms();
    if (init_philosophers(&data) != 0)
        return (1);
    for (int i = 0; i < data.num_philos; i++)
        pthread_join(data.philosophers[i].thread, NULL);
    cleanup(&data);
    return (0);
}
