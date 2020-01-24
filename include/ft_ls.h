/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 00:06:17 by archid-           #+#    #+#             */
/*   Updated: 2020/01/25 23:26:39 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
#define FT_LS_H

#include "file.h"

enum e_ls_flags
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

void	put_flag(bool flag);
struct s_flags
{

	bool list;					/* -l */
	bool one_per_line;			/* -1 */
	bool not_owner;				/* -g */
	bool dir_order;				/* -U */
	bool flag_f;				/* -f
								 * enables -aU, disable -ls --color */
	bool list_dirs;				/* -d
								 * do not dereference links*/
	bool show_all;				/* -a */
	bool show_almost;			/* -A */
	bool recursive;				/* -R */

	bool sort_rev;				/* -r */
	bool sort_mod_time;			/* -t */
	bool sort_acc_time;			/* -u
								 * if -lt ignore t.
								 * if -l, show access sort by ascii */
	bool human_size;			/* -h */


};

void						ft_ls(const char *path, t_flags *flags);
t_queue						*handle_sort(t_queue **aqueue, t_flags *flags);

#endif
