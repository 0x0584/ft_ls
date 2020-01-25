/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 01:02:15 by archid-           #+#    #+#             */
/*   Updated: 2020/01/26 00:11:31 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include "colors.h"

char	*get_file_color(struct stat s)
{
	if (FILE_TYPE(s, S_IFBLK) || FILE_TYPE(s, S_IFCHR)
			|| FILE_TYPE(s, S_IFIFO) || FILE_TYPE(s, S_IFSOCK))
		return (get_color_name(BLINK));
	else if (FILE_TYPE(s, S_IFDIR))
		return (get_color_name(BOLD));
	else if (FILE_TYPE(s, S_IFLNK))
		return (get_color_name(FG_CYAN));
	else if ((s.st_mode >> 6) & 1 || (s.st_mode >> 3) & 1 || s.st_mode & 1)
		return (get_color_name(FG_MAGENTA));
	return (get_color_name(DIM));
}

bool	file_init(t_file *file, const char *path, const char *name,
						bool get_link_info)
{
	if (name)
	{
		file->path = ft_strjoin(path, "/");
		ft_strappend(&file->path, name);
		file->name = ft_strdup(name);
	}
	else
	{
		file->path = ft_strdup(path);
		file->name = ft_strdup(path);
	}
	file->islnk = false;
	if (get_link_info && stat(file->path, &file->st) == -1)
		return (false);
	else if (lstat(file->path, &file->st) == -1)
		return (false);
	else if (FILE_TYPE(file->st, S_IFLNK))
		file->islnk = true;
	file->color = get_file_color(file->st);
	file->pwd = getpwuid(file->st.st_uid);
	file->grp = getgrgid(file->st.st_gid);
	g_uid_width = MAX((int)ft_strlen(file->pwd->pw_name), g_uid_width);
	g_gid_width = MAX((int)ft_strlen(file->grp->gr_name), g_gid_width);
	return (true);
}

char	*get_full_path(t_file *file)
{
	static char buffer[2048];

	ft_strcpy(buffer, file->path);
	ft_strcpy(buffer, "/");
	ft_strcpy(buffer, file->name);
	return (buffer);
}

char	*get_file_name(t_file *file)
{
	static char name[256 + 32];

	ft_bzero(name, 256 + 32);
	ft_snprintf(name, 256 + 32, "%%{%s}%s%%{%s}", file->color,
				file->name, "reset");
	ft_snprintf(name, 256 + 32, name);
	return (name);
}

void	file_del(t_file **f)
{
	if (!f || !*f)
		return ;
	ft_strdel(&(*f)->path);
	ft_strdel(&(*f)->name);
	free(*f);
	*f = NULL;
}

void	queue_file_del(void *e, size_t size)
{
	t_file *f;

	if (!size)
		return ;
	f = e;
	file_del(&f);
}
