/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 00:06:17 by archid-           #+#    #+#             */
/*   Updated: 2020/01/26 00:35:00 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include "file.h"

enum					e_ls_flags
{
	FLAG_LIST = 'l',
	FLAG_ONE_PER_LINE = '1',

	FLAG_SHOW_ALL = 'a',
	FLAG_SHOW_ALMOST_ALL = 'A',

	FLAG_RECURSIVE = 'R',

	FLAG_SORT_REV = 'r',
	FLAG_SORT_ACC_TIME = 'u',
	FLAG_SORT_MOD_TIME = 't',

	FLAG_HUMAN_SIZE = 'h'
};

struct					s_flags
{
	bool list;
	bool one_per_line;
	bool not_owner;
	bool dir_order;
	bool flag_f;

	bool list_dirs;

	bool show_all;
	bool show_almost;
	bool recursive;

	bool sort_rev;
	bool sort_mod_time;
	bool sort_acc_time;

	bool human_size;
};

void					ft_ls(const char *path, t_flags *flags);
t_queue					*handle_sort(t_queue **aqueue, t_flags *flags);

#endif
