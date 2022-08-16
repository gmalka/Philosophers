/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmalka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 12:47:55 by gmalka            #+#    #+#             */
/*   Updated: 2022/03/02 14:52:47 by gmalka           ###   ########.fr       */
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
# include <semaphore.h>
# include <signal.h>

typedef struct philosopher {
	struct philosopher	*next;
	struct philosopher	*pre;
	long int			die;
	int					eat;
	int					sleep;
	int					eat_count;
	int					phl_count;
	int					id;
	int					is_thinking;
	int					is_eating;
	int					fork;
	long int			time;
	long int			ctime;
	sem_t				*sem;
	sem_t				*get;
	sem_t				*gen_sem;
}	t_phls;

t_phls		*make_army_of_philosophers(char *argv[]);

t_phls		*free_all(t_phls *p);

long int	time_now(void);

void		end_of_this_sheet(t_phls *count);

void		*create_philosopher(void *phil);

void		die_of_philosopher(t_phls *phil);

t_phls		*get_pre_phls(t_phls *p);

#endif
