# c-philosophers

This project simulates a dining philosophers problem using threads and mutexes (main) and processes and semaphores (bonus). The goal is to model the life cycle of philosophers who are trying to eat while avoiding starvation and deadlock, with the system enforcing proper synchronization between threads or processes.

## Arguments:
- `number_of_philosophers`: The number of philosophers (and forks) involved in the simulation.
- `time_to_die` (in ms): If a philosopher doesn't start eating within `time_to_die` ms since the start of their last meal or the simulation, they will die.
- `time_to_eat` (in ms): The time a philosopher needs to eat. During this time, they will occupy two forks.
- `time_to_sleep` (in ms): The time a philosopher spends sleeping.
- `number_of_times_each_philosopher_must_eat` (optional): If set, the simulation will stop once every philosopher has eaten at least this many times. If not provided, the simulation stops upon the death of a philosopher.

## Program Flow:
- Each philosopher is assigned a number from 1 to `number_of_philosophers`.
- The philosopher numbered 1 sits next to philosopher numbered `number_of_philosophers`, while others sit between the philosopher numbered N-1 and N+1.
    
# Main Part: `philo`
## Description:
Philosophers implementation using threads and mutexes.
  
# Bonus Part: `philo_bonus`
## Description:
Philosophers implementation using processes and semaphores.