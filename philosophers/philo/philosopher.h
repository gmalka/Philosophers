/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmalka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 12:47:55 by gmalka            #+#    #+#             */
/*   Updated: 2022/03/01 18:40:24 by gmalka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>

typedef struct philosopher {
	struct philosopher	*next;
	struct philosopher	*pre;
	long int			die;
	int					isdie;
	int					eat;
	int					sleep;
	int					eat_count;
	int					phl_count;
	int					id;
	int					is_thinking;
	int					is_eating;
	int					eat_time;
	long int			time;
	long int			ctime;
	pthread_t			*th;
	pthread_mutex_t		*mutex;
	pthread_mutex_t		*general_mutex;
	pthread_mutex_t		*isdead;
}	t_phls;

t_phls		*make_army_of_philosophers(char *argv[]);

t_phls		*free_all(t_phls *p, pthread_t *th);

long int	time_now(void);

int			end_of_this_sheet(t_phls *count);

void		*create_philosopher(void *phil);

int			die_of_philosopher(t_phls *phil);

t_phls		*get_pre_phls(t_phls *p);

int			time_for_sleep(t_phls *phil);

int			is_endeat(t_phls *count, int t);

#endif
