/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phls_movements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmalka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 12:57:35 by gmalka            #+#    #+#             */
/*   Updated: 2022/03/03 08:22:12 by gmalka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	time_for_eat(t_phls *phil)
{
	t_phls			*count;
	long int		time;

	count = phil;
	count->is_thinking = 0;
	time = time_now();
	if (pthread_mutex_lock(count->general_mutex) != 0)
		return (0);
	if (count->isdie == 0)
		printf("%ld %d is eating\n", time - count->time, count->id);
	if (pthread_mutex_unlock(count->general_mutex) != 0)
		return (0);
	if (count->isdie || (count->eat_count > 0
			&& is_endeat(count, ++count->eat_time)))
		end_of_this_sheet(count);
	if ((time_now() - count->ctime) > count->die)
		return (die_of_philosopher(count));
	time = time_now();
	while (time_now() - time < count->eat && usleep(10) == 0)
		if (count->isdie == 1 || time_now() - count->ctime > count->die)
			return (die_of_philosopher(count));
	count->ctime = time_now();
	return (1);
}

static int	take_fork(t_phls *phil)
{
	t_phls			*count;
	long int		time;

	count = phil;
	if (time_now() - count->ctime > count->die)
		return (die_of_philosopher(count));
	time = time_now();
	if (count->isdie == 1 || pthread_mutex_lock(count->general_mutex) != 0)
		return (0);
	if (count->isdie == 0)
		printf("%ld %d has taken a fork\n", time - count->time, count->id);
	if (count->isdie == 1 || pthread_mutex_unlock(count->general_mutex) != 0)
		return (0);
	return (1);
}

static int	time_for_think(t_phls *phil)
{
	t_phls			*count;
	long int		time;

	count = phil;
	if (count->isdie == 0 && count->is_thinking == 0)
	{
		count->is_thinking = 1;
		time = time_now();
		if (pthread_mutex_lock(count->general_mutex) != 0)
			return (0);
		if (count->isdie == 0)
			printf("%ld %d is thinking\n", (time - count->time), count->id);
		if (pthread_mutex_unlock(count->general_mutex) != 0)
			return (0);
	}
	else if (count->isdie == 1)
		return (0);
	if ((time_now() - count->ctime) > count->die)
		return (die_of_philosopher(count));
	return (1);
}

static void	*do_while_in_philosopher(t_phls *count, t_phls *pre)
{
	while (count->isdie == 0)
	{
		if (count->isdie == 1 || (pthread_mutex_lock(count->mutex) == 0
				&& take_fork(count) == 1))
		{
			if (count->isdie == 1 || (pthread_mutex_lock(pre->mutex) == 0
					&& take_fork(count) == 1
					&& time_for_eat(count) == 1
					&& pthread_mutex_unlock(pre->mutex) == 0
					&& pthread_mutex_unlock(count->mutex) == 0
					&& time_for_sleep(count) == 1))
				time_for_think(count);
		}
		else
			return (NULL);
	}
	return (NULL);
}

void	*create_philosopher(void *phil)
{
	t_phls		*count;
	t_phls		*pre;

	count = (t_phls *)phil;
	pre = get_pre_phls(count);
	if (count->id % 2 != 0 && time_for_think(count))
		while (time_now() - count->time < count->eat / 2 + 1)
			usleep(50);
	if ((count->id == pre->id && time_for_think(count) == 1)
		&& take_fork(count) == 1)
		while (count->id == pre->id)
			if (time_for_think(count) == 0)
				return (NULL);
	do_while_in_philosopher(count, pre);
	return (NULL);
}
