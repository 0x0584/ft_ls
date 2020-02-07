/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_extra.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/26 04:30:43 by archid-           #+#    #+#             */
/*   Updated: 2020/01/26 04:31:37 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		do_sort_ascii(t_qnode *e1, t_qnode *e2, bool rev)
{
	t_file	*f1;
	t_file	*f2;
	int		sign;

	f1 = e1->blob;
	f2 = e2->blob;
	sign = rev ? -1 : 1;
	return (sign * ft_strcmp(f1->name, f2->name));
}

int		do_sort_mod_time(t_qnode *e1, t_qnode *e2, bool rev)
{
	t_file	*f1;
	t_file	*f2;
	int		sign;

	f1 = e1->blob;
	f2 = e2->blob;
	sign = rev ? -1 : 1;
	return (sign * (f2->st.st_mtime - f1->st.st_mtime));
}

int		do_sort_acc_time(t_qnode *e1, t_qnode *e2, bool rev)
{
	t_file	*f1;
	t_file	*f2;
	int		sign;

	f1 = e1->blob;
	f2 = e2->blob;
	sign = rev ? -1 : 1;
	return (sign * (f2->st.st_atime - f1->st.st_atime));
}

int		sort_ascii_desc(t_qnode *e1, t_qnode *e2)
{
	return (do_sort_ascii(e1, e2, true));
}

int		sort_ascii_asc(t_qnode *e1, t_qnode *e2)
{
	return (do_sort_ascii(e1, e2, false));
}
