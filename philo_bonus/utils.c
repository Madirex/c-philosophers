#include "philo.h"

int	ft_atoi(const char *str)
{
	int	sign;
	int	value;

	sign = 1;
	value = 0;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
		sign = 44 - *str++;
	while (*str >= '0' && *str <= '9')
		value = value * 10 + (*str++ - '0');
	return (sign * value);
}

int	ft_strcmp(const char *str1, const char *str2)
{
	while (*str1 && *str2 && *str1 == *str2)
	{
		str1++;
		str2++;
	}
	return (*(unsigned char *)str1 - *(unsigned char *)str2);
}
