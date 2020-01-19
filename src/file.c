/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 01:02:15 by archid-           #+#    #+#             */
/*   Updated: 2020/01/19 22:31:07 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include "colors.h"

char *get_full_path(t_file *file)
{
	static char buffer[2048];

	ft_strcpy(buffer, file->path);
	ft_strcpy(buffer, "/");
	ft_strcpy(buffer, file->name);
	return (buffer);
}

char	*get_file_color(struct stat s)
{
	if (FILE_TYPE(s, S_IFBLK) || FILE_TYPE(s, S_IFCHR)
			|| FILE_TYPE(s, S_IFIFO) || FILE_TYPE(s, S_IFSOCK))
		return get_color_name(BLINK);
	else if (FILE_TYPE(s, S_IFDIR))
		return get_color_name(BOLD);
	else if (FILE_TYPE(s, S_IFLNK))
		return get_color_name(FG_RED);
	else if ((s.st_mode >> 6) & 1 || (s.st_mode >> 3) & 1 || s.st_mode & 1)
		return get_color_name(FG_MAGENTA);
	return get_color_name(DIM);
}

char	*get_file_name(t_file *file)
{
	static char name[256 + 32];

	ft_bzero(name, 256 + 32);
	ft_snprintf(name, 256 + 32, "%%{%s}%s%%{%s}", file->color,
				file->name, "reset");
	ft_snprintf(name, 256 + 32, name);
	return name;
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
		file->path = ft_strjoin(path, "/");
		ft_strappend(&file->path, name);
		file->name = ft_strdup(name);
	}
	if (get_link_info)
	{
		if (stat(file->path, &file->st) == -1)
			return false;
		lstat(file->path, &file->lnk);
		file->islink = false;
	}
	else
	{
		if (lstat(file->path, &file->st) == -1)
			return false;
		file->islink = FILE_TYPE(file->st, S_IFLNK);
	}
	file->color = get_file_color(file->st);
	return (true);
}
