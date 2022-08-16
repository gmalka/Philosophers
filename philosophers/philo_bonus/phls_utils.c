/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phls_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmalka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 13:10:45 by gmalka            #+#    #+#             */
/*   Updated: 2022/03/03 08:00:16 by gmalka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

t_phls	*free_all(t_phls *p)
{
	t_phls	*count;

	while (p != NULL)
	{
		count = p->next;
		free(p);
		p = count;
	}
	p = NULL;
	return (NULL);
}

long int	time_now(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	end_of_this_sheet(t_phls *count)
{
	count->eat_count = 0;
	sem_post(count->get);
	return ;
}

void	die_of_philosopher(t_phls *phil)
{
	t_phls			*count;
	long int		time;

	count = phil;
	time = time_now();
	sem_wait(count->gen_sem);
	printf("%ld %d died ☠️  in %ld\n", time - count->time,
		count->id, time - count->ctime);
	usleep(10);
	exit(0);
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
