/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 00:06:17 by archid-           #+#    #+#             */
/*   Updated: 2020/01/19 14:39:19 by archid-          ###   ########.fr       */
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

	FLAG_RECURSIVE = 'R',

	FLAG_SORT_REV = 'r',
	FLAG_SORT_ACC_TIME = 'u',
	FLAG_SORT_MOD_TIME = 't',

	FLAG_HUMAN_SIZE = 'h'
};

typedef struct s_flags t_flags;
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
	bool recursive;				/* -R */

	bool sort_rev;				/* -r */
	bool sort_mod_time;			/* -t */
	bool sort_acc_time;			/* -u
								 * if -lt ignore t.
								 * if -l, show access sort by ascii */
	bool human_size;

};

typedef struct s_ls_info t_info;
struct s_ls_info
{
	t_queue		*files;
	int			link_offset;
	int			size_offset;
};

void	ft_ls(const char *path, t_flags *flags);
t_lst	handle_sort(t_lst *alst, t_flags *flags);

#endif
