/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 17:28:35 by archid-           #+#    #+#             */
/*   Updated: 2020/01/26 18:21:09 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int			sort_acc_time_desc(t_qnode *e1, t_qnode *e2)
{
	return (do_sort_acc_time(e1, e2, true));
}

int			sort_acc_time_asc(t_qnode *e1, t_qnode *e2)
{
	return (do_sort_acc_time(e1, e2, false));
}

int			sort_mod_time_desc(t_qnode *e1, t_qnode *e2)
{
	return (do_sort_mod_time(e1, e2, true));
}

int			sort_mod_time_asc(t_qnode *e1, t_qnode *e2)
{
	return (do_sort_mod_time(e1, e2, false));
}

t_queue		*handle_sort(t_queue **aqueue, t_flags *flags)
{
	if (flags->sort_acc_time)
		queue_mergesort(aqueue, flags->sort_rev
							? sort_acc_time_desc : sort_acc_time_asc);
	else if (flags->sort_mod_time)
		queue_mergesort(aqueue, flags->sort_rev
							? sort_mod_time_desc : sort_mod_time_asc);
	else
		queue_mergesort(aqueue, flags->sort_rev
							? sort_ascii_desc : sort_ascii_asc);
	return (*aqueue);
}
