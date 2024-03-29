/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midfath <midfath@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 00:33:04 by midfath           #+#    #+#             */
/*   Updated: 2022/08/16 18:44:09 by midfath          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_header.h"

int	ft_endthreads(t_parma *p)
{
	int	i;

	i = 0;
	while (i < p->n_philo)
	{
		pthread_join(p->philo[i].t_id, NULL);
		pthread_mutex_destroy(&p->l);
		pthread_mutex_destroy(&p->key[i]);
		i++;
	}
	pthread_mutex_destroy(&p->output_key);
	free(p->key);
	free(p->philo);
	free(p);
	p->key = NULL;
	p->philo = NULL;
	p = NULL;
	return (0);
}

int	ft_dining_philos(t_parma *p)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	p->philo = malloc(p->n_philo * sizeof(t_philo));
	p->key = malloc(p->n_philo * sizeof(pthread_mutex_t));
	if (!p->philo || !p->key)
		return (1);
	while (i < p->n_philo)
	{
		p->philo[i].id = i;
		p->philo[i].t_life = 1;
		p->philo[i].n_toeat = 0;
		p->philo[i].t_death = p->t_spawn;
		i++;
	}
	if (ft_start_serving(p))
		return (1);
	else if (ft_spawn_philos(p))
		return (1);
	return (0);
}

int	ft_spawn_philos(t_parma *p)
{
	int			i;
	t_philo		*ph;

	i = 0;
	p->t_spawn = ft_time(NULL);
	while (i < p->n_philo)
	{
		ph = &p->philo[i];
		ph->pram = p;
		if (pthread_create(&(p->philo[i].t_id), NULL, ft_start, ph))
			return (1);
		i++;
		usleep(100);
	}
	return (0);
}

void	ft_track(t_parma *p)
{
	int	i;

	while (f_race(p))
	{
		i = 0;
		p->p_full = 0;
		while (p->n_eat >= 0 && i < p->n_philo && !p->p_end)
		{
			pthread_mutex_lock(&p->l);
			if (p->philo[i].n_toeat < p->n_eat)
			{
				pthread_mutex_unlock(&p->l);
				usleep(100);
				break ;
			}
			pthread_mutex_unlock(&p->l);
			i++;
			p->p_full++;
		}
		if (p->p_full == p->n_philo)
			p->p_end = 1;
		ft_dead(p);
	}		
}
