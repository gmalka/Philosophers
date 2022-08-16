/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phls_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmalka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 13:10:45 by gmalka            #+#    #+#             */
/*   Updated: 2022/03/02 12:07:32 by gmalka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

t_phls	*free_all(t_phls *p, pthread_t *th)
{
	t_phls	*count;

	while (p != NULL)
	{
		count = p->next;
		free(p);
		p = count;
	}
	if (th != NULL)
		free(th);
	p = NULL;
	return (NULL);
}

long int	time_now(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

int	end_of_this_sheet(t_phls *count)
{
	long int	time;

	time = time_now();
	if (count->isdie == 1)
		return (0);
	if (pthread_mutex_lock(count->general_mutex) != 0)
		return (0);
	if (count->isdie == 0)
		printf("%ld They all die! No hope, it's End!!!\n", (time - count->time));
	if (pthread_mutex_unlock(count->isdead) != 0)
		return (0);
	return (0);
}

int	die_of_philosopher(t_phls *phil)
{
	t_phls			*count;
	long int		time;

	count = phil;
	time = time_now();
	if (count->isdie == 1)
		return (0);
	if (pthread_mutex_lock(count->general_mutex) != 0)
		return (0);
	if (count->isdie == 0)
		printf("%ld %d died ☠️  in %ld\n", time - count->time,
			count->id, time - count->ctime);
	if (pthread_mutex_unlock(count->isdead) != 0)
		return (0);
	usleep(500);
	return (0);
}

t_phls	*get_pre_phls(t_phls *p)
{
	t_phls	*count;

	if (p->pre)
		return (p->pre);
	count = p;
	while (count->next)
		count = count->next;
	return (count);
}
