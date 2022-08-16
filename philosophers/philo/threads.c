/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmalka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 13:14:30 by gmalka            #+#    #+#             */
/*   Updated: 2022/03/01 18:12:32 by gmalka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	ft_atoi(const char *str)
{
	int			i;
	int			count;
	long int	j;

	i = 0;
	j = 0;
	count = -1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			count = i++;
		else
			i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
		j = (j * 10) + (str[i++] - '0');
	if (count >= 0)
		return (j * (-1));
	return (j);
}

static t_phls	*make_new_philosopher(char **argv, int id, pthread_mutex_t *m)
{
	t_phls		*new;

	new = malloc(sizeof(t_phls));
	if (new == NULL)
		return (NULL);
	new->die = ft_atoi(argv[2]);
	new->eat = ft_atoi(argv[3]);
	new->sleep = ft_atoi(argv[4]);
	if (new->die == 0 || new->eat == 0 || new->sleep == 0)
		return (NULL);
	if (argv[5] != NULL)
		new->eat_count = ft_atoi(argv[5]);
	else
		new->eat_count = -1;
	new->next = NULL;
	new->pre = NULL;
	new->id = id;
	new->isdie = 0;
	new->is_thinking = 0;
	new->is_eating = 0;
	new->eat_time = 0;
	new->general_mutex = m;
	return (new);
}

static t_phls	*more_philosopher(int i,
	pthread_mutex_t *general_mutex, char *argv[])
{
	t_phls				*philosopher;
	t_phls				*counter;
	t_phls				*pre;
	int					j;

	j = 0;
	philosopher = make_new_philosopher(argv, ++j, general_mutex);
	if (!philosopher)
		return (0);
	philosopher->phl_count = i;
	counter = philosopher;
	pre = NULL;
	while (--i > 0)
	{
		counter->next = make_new_philosopher(argv, ++j, general_mutex);
		pre = counter;
		counter = counter->next;
		counter->pre = pre;
		if (counter == NULL)
			return (free_all(philosopher, NULL));
	}
	return (philosopher);
}

t_phls	*make_army_of_philosophers(char *argv[])
{
	int					i;
	t_phls				*philosopher;
	pthread_mutex_t		*general_mutex;

	general_mutex = malloc(sizeof(pthread_mutex_t));
	if (general_mutex == NULL)
		return (0);
	pthread_mutex_init(general_mutex, NULL);
	i = ft_atoi(argv[1]);
	if (i <= 0)
		return (NULL);
	philosopher = more_philosopher(i, general_mutex, argv);
	if (!philosopher)
		return (0);
	return (philosopher);
}
