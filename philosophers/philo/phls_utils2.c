/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phls_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmalka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:23:13 by gmalka            #+#    #+#             */
/*   Updated: 2022/03/02 14:18:18 by gmalka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	time_for_sleep(t_phls *phil)
{
	t_phls			*count;
	long int		time;

	count = phil;
	time = time_now();
	if (pthread_mutex_lock(count->general_mutex) != 0)
		return (0);
	if (count->isdie == 0)
		printf("%ld %d is sleeping\n", time - count->time, count->id);
	if (count->isdie == 1 || pthread_mutex_unlock(count->general_mutex) != 0)
		return (0);
	count->is_thinking = 0;
	time = time_now();
	while (time_now() - time < count->sleep)
	{
		usleep(10);
		if (count->isdie == 1 || time_now() - count->ctime > count->die)
			return (die_of_philosopher(count));
	}
	return (1);
}

int	is_endeat(t_phls *count, int t)
{
	t_phls	*next;
	t_phls	*pre;

	pthread_mutex_lock(count->general_mutex);
	if (t < count->eat_count)
		return (pthread_mutex_unlock(count->general_mutex));
	next = count->next;
	pre = count->pre;
	while (next)
	{
		if (next->eat_time < next->eat_count)
			return (pthread_mutex_unlock(count->general_mutex));
		next = next->next;
	}
	while (pre)
	{
		if (pre->eat_time < pre->eat_count)
			return (pthread_mutex_unlock(count->general_mutex));
		pre = pre->pre;
	}
	return (pthread_mutex_unlock(count->general_mutex) + 1);
}
