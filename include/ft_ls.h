/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 00:06:17 by archid-           #+#    #+#             */
/*   Updated: 2020/02/06 21:00:27 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include "file.h"

enum						e_ls_flags
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

struct						s_flags
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

struct						s_qarray
{
	t_qnode		**base;
	size_t		size;
};

void						ft_ls(const char *path, t_flags *flags);
t_queue						*handle_sort(t_queue **aqueue, t_flags *flags);
void						display_files(t_queue **files, t_flags *flags);

bool						user_has_permission(const char *path,
													struct stat st);
void						raise_error(t_queue **files, const char *path);
void						set_widths(struct stat st, const char *name);
void						reset_widths(void);
bool						set_stat(t_file *file, const char *path,
										t_flags *flags, bool *is_there_error);

int							do_sort_ascii(t_qnode *e1, t_qnode *e2, bool rev);
int							do_sort_mod_time(t_qnode *e1, t_qnode *e2,
												bool rev);
int							do_sort_acc_time(t_qnode *e1, t_qnode *e2,
												bool rev);

int							sort_ascii_desc(t_qnode *e1, t_qnode *e2);
int							sort_ascii_asc(t_qnode *e1, t_qnode *e2);
int							sort_acc_time_desc(t_qnode *e1, t_qnode *e2);
int							sort_acc_time_asc(t_qnode *e1, t_qnode *e2);
int							sort_mod_time_desc(t_qnode *e1, t_qnode *e2);
int							sort_mod_time_asc(t_qnode *e1, t_qnode *e2);

void						enqueue_dirs(t_queue *dirs, t_file *file,
											t_flags *flags);
size_t						get_step(size_t size);
int							queue_strcmp(t_qnode *u, t_qnode *v);
void						print_usage(char option, char *prog_name);

#endif
