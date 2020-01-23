/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 17:28:35 by archid-           #+#    #+#             */
/*   Updated: 2020/01/23 21:49:18 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		__sort_ascii(t_qnode *e1, t_qnode *e2, bool rev)
{
	t_file	*f1;
	t_file	*f2;
	int		sign;

	f1 = e1->blob;
	f2 = e2->blob;
	sign = rev ? -1 : 1;
	return sign * ft_strcmp(f1->name, f2->name);
}

int		__sort_mod_time(t_qnode *e1, t_qnode *e2, bool rev)
{
	t_file *f1;
	t_file *f2;
	int sign;

	f1 = e1->blob;
	f2 = e2->blob;
	sign = rev ? -1 : 1;
	return sign * (f2->st.st_mtime - f1->st.st_mtime);
}

int		__sort_acc_time(t_qnode *e1, t_qnode *e2, bool rev)
{
	t_file	*f1;
	t_file	*f2;
	int		sign;

	f1 = e1->blob;
	f2 = e2->blob;
	sign = rev ? -1 : 1;
	return sign * (f2->st.st_atime - f1->st.st_atime);
}


int		sort_ascii_desc(t_qnode *e1, t_qnode *e2)
{
	return __sort_ascii(e1, e2, true);
}

int		sort_ascii_asc(t_qnode *e1, t_qnode *e2)
{
	return __sort_ascii(e1, e2, false);
}

int		sort_acc_time_desc(t_qnode *e1, t_qnode *e2)
{
	return __sort_acc_time(e1, e2, true);
}

int		sort_acc_time_asc(t_qnode *e1, t_qnode *e2)
{
	return __sort_acc_time(e1, e2, false);
}

int		sort_mod_time_desc(t_qnode *e1, t_qnode *e2)
{
	return __sort_mod_time(e1, e2, true);
}

int		sort_mod_time_asc(t_qnode *e1, t_qnode *e2)
{
 	return __sort_mod_time(e1, e2, false);
}

t_queue *handle_sort(t_queue **aqueue, t_flags *flags)
{
	// ft_putendl(" ,,,  ");

	if (flags->sort_acc_time)
		queue_mergesort(aqueue, flags->sort_rev
							? sort_acc_time_desc : sort_acc_time_asc);
	else if (flags->sort_mod_time)
		queue_mergesort(aqueue, flags->sort_rev
							? sort_mod_time_desc : sort_mod_time_asc);
	else
	{
		queue_mergesort(aqueue, flags->sort_rev
							? sort_ascii_desc : sort_ascii_asc);
		// ft_putendl(" lll  ");

	}
	// ft_putendl(" ,,,  ");
	return *aqueue;
}
