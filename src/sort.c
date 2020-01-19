/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 17:28:35 by archid-           #+#    #+#             */
/*   Updated: 2020/01/19 14:35:11 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		sort_ascii(t_lst e1, t_lst e2)
{
	t_file *f1;
	t_file *f2;

	f1 = e1->content;
	f2 = e2->content;
	return ft_strcmp(f1->name, f2->name);
}

int		sort_ascii_reversed(t_lst e1, t_lst e2)
{
	t_file *f1;
	t_file *f2;

	f1 = e1->content;
	f2 = e2->content;
	return ft_strcmp(f2->name, f1->name);
}

int		sort_mod_time(t_lst e1, t_lst e2)
{
	t_file *f1;
	t_file *f2;

	f1 = e1->content;
	f2 = e2->content;
	return f1->st.st_mtime - f2->st.st_mtime;
}

int		sort_acc_time(t_lst e1, t_lst e2)
{
	t_file *f1;
	t_file *f2;

	f1 = e1->content;
	f2 = e2->content;
	return f1->st.st_atime - f2->st.st_atime;
}

t_lst	handle_sort(t_lst *alst, t_flags *flags)
{
	if (flags->sort_rev)
		ft_lst_mergesort(alst, sort_ascii_reversed);
	else if (flags->sort_mod_time)
		ft_lst_mergesort(alst, sort_mod_time);
	else if (flags->sort_acc_time)
		ft_lst_mergesort(alst, sort_acc_time);
	else
		ft_lst_mergesort(alst, sort_ascii);
	return *alst;
}
