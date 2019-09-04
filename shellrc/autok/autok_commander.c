/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autok_commander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pharatyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 14:46:03 by pharatyk          #+#    #+#             */
/*   Updated: 2019/07/01 14:46:05 by pharatyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	ft_commander_1opt(t_keymaster *lok, int i)
{
	i = -1;
	while (lok->fcts[0][++i])
		if (ft_strchrstrfirst(lok->fcts[0][i], lok->final))
		{
			if (!ft_strcmp(lok->fcts[0][i], lok->final))
			{
				if (!(lok->complete = (char*)malloc(sizeof(char) * 2)))
					return (1);
				lok->complete[0] = ' ';
				lok->complete[1] = 0;
			}
			else
			{
				if (!(lok->complete = (char*)malloc(sizeof(char)
					* (2 + ft_strlen(lok->fcts[0][i])
						- ft_strlen(lok->final)))))
					return (1);
				ft_strcpy(lok->complete, lok->fcts[0][i]
					+ ft_strlen(lok->final));
				ft_strcat(lok->complete, " ");
			}
			free(lok->final);
			break ;
		}
	return (0);
}

static int	ft_commander_multiopt(t_keymaster *lok, int i)
{
	if (!(lok->stock.output = (char**)malloc(sizeof(char*)
		* (lok->nb_auto + 1))))
		return (1);
	lok->stock.output[lok->nb_auto] = 0;
	lok->stock.argc = lok->nb_auto;
	lok->stock.select = 0;
	i = -1;
	lok->nb_auto = 0;
	while (lok->fcts[0][++i])
		if (ft_strchrstrfirst(lok->fcts[0][i], lok->final))
		{
			if (!(lok->stock.output[lok->nb_auto] =
					(char*)malloc(sizeof(char)
						* (ft_strlen(lok->fcts[0][i]) + 1))))
				return (1);
			ft_strcpy(lok->stock.output[lok->nb_auto], lok->fcts[0][i]);
			lok->nb_auto += 1;
		}
	return (0);
}

static int	ft_commander_multioptbasic(t_keymaster *lock, t_stockap *pac
	, int i)
{
	int			savepos;
	t_basiks	tools;

	savepos = lock->curspos;
	ft_simpleputatend(lock, pac);
	ft_printfd(0, "\n");
	if (ft_basic(&lock->stock, pac, &tools, lock))
		return (1);
	lock->aukprinted = 3;
	i = -2;
	while (++i < (tools.nbligne + lock->lvl))
		ft_printfd(0, "%s", pac->up_cap);
	lock->curspos = 0;
	i = -1;
	while (++i < pac->nbcol)
		ft_printfd(0, "%s", pac->left_cap);
	i = -1;
	while (++i < savepos)
		ft_simplegoright(lock, pac);
	return (0);
}

static int	ft_commander_end(t_keymaster *lock, t_stockap *pac)
{
	int i;

	i = 0;
	if (lock->nb_auto == 1)
	{
		if (ft_commander_1opt(lock, i))
			return (1);
	}
	else if (lock->nb_auto > 1)
	{
		if (ft_commander_multiopt(lock, i))
			return (1);
		if (ft_commander_multioptbasic(lock, pac, i))
			return (1);
	}
	if (lock->nb_auto == 1)
		ft_addcompletion(lock, pac);
	if (lock->nb_auto == 0)
		free(lock->final);
	return (0);
}

int			ft_commander(t_keymaster *lock, t_stockap *pac)
{
	int		i;

	if (!(lock->final = (char*)malloc(sizeof(char)
		* (ft_strlen(lock->tmp) + 1))))
		return (1);
	ft_strcpy(lock->final, lock->tmp);
	free(lock->tmp);
	lock->nb_auto = 0;
	i = -1;
	while (lock->fcts[0][++i])
		if (ft_strchrstrfirst(lock->fcts[0][i], lock->final))
			lock->nb_auto += 1;
	return (ft_commander_end(lock, pac));
}
