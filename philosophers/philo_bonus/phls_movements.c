/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phls_movements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmalka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 12:57:35 by gmalka            #+#    #+#             */
/*   Updated: 2022/03/03 07:59:54 by gmalka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	time_for_eat(t_phls *phil, int *i)
{
	t_phls			*count;
	long int		time;

	count = phil;
	count->is_thinking = 0;
	time = time_now();
	sem_wait(count->gen_sem);
	printf("%ld %d is eating\n", time - count->time, count->id);
	sem_post(count->gen_sem);
	if (count->eat_count > 0 && ++(*i) >= count->eat_count)
		end_of_this_sheet(count);
	if ((time_now() - count->ctime) > count->die)
		die_of_philosopher(count);
	time = time_now();
	while (time_now() - time < count->eat)
	{
		usleep(10);
		if (time_now() - count->ctime > count->die)
			die_of_philosopher(count);
	}
	count->ctime = time_now();
	return (1);
}

static int	time_for_sleep(t_phls *phil)
{
	t_phls			*count;
	long int		time;

	count = phil;
	time = time_now();
	sem_wait(count->gen_sem);
	printf("%ld %d is sleeping\n", time - count->time, count->id);
	sem_post(count->gen_sem);
	count->is_thinking = 0;
	time = time_now();
	while (time_now() - time < count->sleep)
	{
		usleep(10);
		if (time_now() - count->ctime > count->die)
			die_of_philosopher(count);
	}
	return (1);
}

static int	take_fork(t_phls *phil)
{
	t_phls			*count;
	long int		time;

	count = phil;
	if (time_now() - count->ctime > count->die)
		die_of_philosopher(count);
	time = time_now();
	sem_wait(count->gen_sem);
	printf("%ld %d has taken a fork\n", time - count->time, count->id);
	sem_post(count->gen_sem);
	return (1);
}

static int	time_for_think(t_phls *phil)
{
	t_phls			*count;
	long int		time;

	count = phil;
	if (count->is_thinking == 0)
	{
		count->is_thinking = 1;
		time = time_now();
		sem_wait(count->gen_sem);
		printf("%ld %d is thinking\n", (time - count->time), count->id);
		sem_post(count->gen_sem);
	}
	if ((time_now() - count->ctime) > count->die)
		die_of_philosopher(count);
	return (1);
}

void	*create_philosopher(void *phil)
{
	int			i;
	t_phls		*count;

	i = 0;
	count = (t_phls *)phil;
	(count)->sem = sem_open("/regul1", 0);
	(count)->gen_sem = sem_open("/gen1", 0);
	(count)->get = sem_open("/get", 0);
	if (count->id % 2 != 0 && time_for_think(count))
		while (time_now() - count->time < count->eat + 1)
			usleep(10);
	if ((count->next == NULL && count->pre == NULL
			&& time_for_think(count) == 1) && take_fork(count) == 1)
		while (1)
			time_for_think(count);
	while (1)
	{
		if (sem_wait(count->sem) == 0 && take_fork(count))
			if (sem_wait(count->sem) == 0 && take_fork(count)
				&& time_for_eat(count, &i) && sem_post(count->sem) == 0
				&& sem_post(count->sem) == 0 && time_for_sleep(count))
				time_for_think(count);
	}
}
