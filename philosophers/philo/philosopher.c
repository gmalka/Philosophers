/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmalka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 12:23:35 by gmalka            #+#    #+#             */
/*   Updated: 2022/03/03 08:21:17 by gmalka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	do_it_for_me(int j, pthread_t *th,
		t_phls *count, pthread_mutex_t *isdead)
{
	pthread_mutex_t	*mutex;
	long int		time;
	int				i;

	mutex = NULL;
	i = -1;
	time = time_now();
	while (++i < j)
	{
		mutex = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(mutex, NULL);
		count->mutex = mutex;
		count->time = time;
		count->ctime = count->time;
		count->isdead = isdead;
		if (pthread_create(&th[i], NULL, &create_philosopher, count) != 0)
			return (write(2, "Pthread create Error\n", 21));
		count = count->next;
	}
	return (0);
}

int	do_pthread(int i, pthread_t *th,
		t_phls *philosopher, pthread_mutex_t *mutex)
{
	int					j;
	pthread_mutex_t		*isdead;
	t_phls				*count;

	isdead = philosopher->isdead;
	j = philosopher->phl_count;
	count = philosopher;
	pthread_mutex_lock(philosopher->general_mutex);
	if (do_it_for_me(j, th, count, isdead) != 0)
		return (0);
	pthread_mutex_unlock(philosopher->general_mutex);
	i = 0;
	if (pthread_join(th[j + 1], NULL) != 0)
		return (write(2, "Pthread join Error\n", 19));
	free_all(philosopher, th);
	return (0);
}

static void	lock_unlock(t_phls *count)
{
	pthread_mutex_unlock(count->isdead);
	pthread_mutex_destroy(count->isdead);
	free(count->isdead);
	pthread_mutex_unlock(count->general_mutex);
	pthread_mutex_destroy(count->general_mutex);
	free(count->general_mutex);
}

static void	*is_it_over(void *philosopher)
{
	t_phls	*count;
	int		i;

	i = -1;
	count = (t_phls *)philosopher;
	pthread_mutex_lock(count->isdead);
	while (++i < count->phl_count)
		pthread_detach(count->th[i]);
	while (count)
	{
		count->isdie = 1;
		count = count->next;
	}
	count = (t_phls *)philosopher;
	pthread_mutex_unlock(count->general_mutex);
	lock_unlock(count);
	while (count && pthread_mutex_unlock(count->mutex) == 0
		&& pthread_mutex_destroy(count->mutex) == 0)
	{
		free(count->mutex);
		count = count->next;
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	int					i;
	pthread_t			*th;
	t_phls				*philosopher;
	pthread_mutex_t		*isdead;

	i = -1;
	if (argc < 5 || argc > 6)
		return (write(2, "Sorry, Need another number of argum \
ents to start philosophizing;(\n", 66));
	isdead = malloc(sizeof(pthread_mutex_t));
	if (!isdead || pthread_mutex_init(isdead, NULL) != 0)
		return (0);
	philosopher = make_army_of_philosophers(argv);
	if (!philosopher)
		return (write(1, "Incorrect argument\n", 19));
	th = malloc(sizeof(pthread_t) * (philosopher->phl_count + 2));
	philosopher->isdead = isdead;
	pthread_mutex_lock(isdead);
	if (th == NULL || pthread_create(&th[philosopher->phl_count + 1],
			NULL, &is_it_over, philosopher) != 0)
		return (write(2, "Malloc Error\n", 13));
	philosopher->th = th;
	do_pthread(i, th, philosopher, NULL);
}
