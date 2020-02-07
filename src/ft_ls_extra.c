/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_extra.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/26 18:17:15 by archid-           #+#    #+#             */
/*   Updated: 2020/02/06 20:55:07 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

bool	user_has_permission(const char *path, struct stat st)
{
	char *tmp;

	if (st.st_mode & S_IRGRP)
		return (true);
	tmp = ft_strrchr(path, '/');
	ft_printf("ls: %s: Permission denied\n", tmp ? tmp + 1 : path);
	return (false);
}

void	raise_error(t_queue **files, const char *path)
{
	if (path)
		perror(path);
	queue_del(files, queue_file_del);
}

void	set_widths(struct stat st, const char *name)
{
	g_column_width = MAX((int)ft_strlen(name) + 1, g_column_width);
	g_link_width = MAX(g_link_width, ft_digitcount(st.st_nlink));
	g_size_width = MAX(g_size_width, ft_digitcount(st.st_size) + 1);
}

void	reset_widths(void)
{
	g_link_width = 1;
	g_size_width = 1;
	g_column_width = 1;
	g_uid_width = 1;
	g_gid_width = 1;
}

bool	set_stat(t_file *file, const char *path, t_flags *flags, bool *flag)
{
	if (lstat(path, &file->st) == -1)
	{
		perror(path);
		*flag = true;
		return (false);
	}
	if (!flags->list && stat(path, &file->st) == -1)
	{
		perror(path);
		*flag = true;
		return (false);
	}
	file_init(file, path, NULL, !flags->list);
	return (true);
}
