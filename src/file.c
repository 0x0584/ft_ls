/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 01:02:15 by archid-           #+#    #+#             */
/*   Updated: 2020/01/17 22:01:45 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char *get_full_path(t_file *file)
{
	static char buffer[2048];

	ft_strcpy(buffer, file->path);
	ft_strcpy(buffer, "/");
	ft_strcpy(buffer, file->name);
	return (buffer);
}

bool	file_init(t_file *file, const char *path, const char *name,
					bool get_link_info)
{
	if (path == name)
	{
		file->path = ft_strdup(path);
		file->name = ft_strdup(name);
	}
	else
	{
		/* ft_printf(" here "); */
		file->path = ft_strjoin(path, "/");
		ft_strappend(&file->path, name);
		file->name = ft_strdup(name);
	}
	if (get_link_info)
	{
		/* ft_printf(" that "); */
		if (stat(file->path, &file->st) == -1)
			return false;
		lstat(file->path, &file->lnk);
		file->islink = false;
	}
	else
	{
		/* ft_printf(" this "); */
		if (lstat(file->path, &file->st) == -1)
			return false;
		file->islink = FILE_TYPE(file->st, S_IFLNK);
	}
	return (true);
}
