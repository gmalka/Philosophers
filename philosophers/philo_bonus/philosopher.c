/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmalka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 12:23:35 by gmalka            #+#    #+#             */
/*   Updated: 2022/03/03 07:59:18 by gmalka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	kill_all(t_phls *phil)
{
	t_phls	*count;

	count = phil;
	while (count)
	{
		kill(count->fork, SIGKILL);
		count = count->next;
	}
}

static void	*check_philosopher(t_phls *phls, sem_t *sem, sem_t *gen_sem)
{
	sem_t		*get;

	get = sem_open("/get", 0);
	waitpid(-1, 0, 0);
	kill_all(phls);
	sem_post(gen_sem);
	sem_unlink("/regul1");
	sem_unlink("/gen1");
	sem_unlink("/get");
	sem_close(sem);
	sem_close(gen_sem);
	sem_close(get);
	exit(0);
}

static void	do_pthread(int i, t_phls *philosopher, int *frk)
{
	t_phls				*count;
	long int			time;
	sem_t				*sem;
	sem_t				*gen_sem;
	int					j;

	j = philosopher->phl_count;
	time = time_now();
	sem = sem_open("/regul1", O_CREAT, 0777, philosopher->phl_count);
	gen_sem = sem_open("/gen1", O_CREAT, 0777, 1);
	time = time_now();
	count = philosopher;
	sem_wait(gen_sem);
	while (++i < j)
	{
		count->time = time;
		count->ctime = philosopher->time;
		frk[i] = fork();
		if ((frk[i] == 0 && create_philosopher(count)) || frk[i] < 0)
			exit(write(2, "Process create Error\n", 21));
		count->fork = frk[i];
		count = count->next;
	}
	sem_post(gen_sem);
	check_philosopher(philosopher, sem, gen_sem);
}

static void	*eat_cnt(void *philosopher)
{
	int				i;
	t_phls			*count;

	count = (t_phls *)philosopher;
	i = -1;
	count->get = sem_open("/get", 0);
	while (++i < count->phl_count)
		sem_wait(count->get);
	sem_wait(count->gen_sem);
	kill(count->fork, SIGKILL);
	printf("%ld They all die! No hope, it's End!!!\n",
		(time_now() - count->time));
	return (NULL);
}

int	main(int argc, char *argv[])
{
	int					i;
	int					*frk;
	pthread_t			th;
	t_phls				*philosopher;

	i = -1;
	sem_unlink("/regul1");
	sem_unlink("/gen1");
	sem_unlink("/get");
	if (argc < 5 || argc > 6)
		return (write(2, "Sorry, Need another number of argum \
ents to start philosophizing;(\n", 66));
	philosopher = make_army_of_philosophers(argv);
	philosopher->get = sem_open("/get", O_CREAT, 0777, philosopher->phl_count);
	while (++i < philosopher->phl_count)
		sem_wait(philosopher->get);
	frk = malloc(philosopher->phl_count * sizeof(int));
	i = -1;
	if (!frk)
		exit(0);
	if (philosopher->eat_count > 0 && pthread_create(&th,
			NULL, &eat_cnt, philosopher) != 0)
		return (write(1, "Pthread Error\n", 14));
	do_pthread(i, philosopher, frk);
	if (philosopher->eat_count > 0 && pthread_join(th, NULL))
		return (0);
}
