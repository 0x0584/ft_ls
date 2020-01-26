/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_info.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 14:38:50 by archid-           #+#    #+#             */
/*   Updated: 2020/01/26 18:48:53 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char	get_file_type(struct stat s)
{
	char type;

	type = '?';
	if (FILE_TYPE(s, S_IFBLK))
		type = 'b';
	else if (FILE_TYPE(s, S_IFCHR))
		type = 'c';
	else if (FILE_TYPE(s, S_IFLNK))
		type = 'l';
	else if (FILE_TYPE(s, S_IFDIR))
		type = 'd';
	else if (FILE_TYPE(s, S_IFIFO))
		type = 'p';
	else if (FILE_TYPE(s, S_IFREG))
		type = '-';
	else if (FILE_TYPE(s, S_IFSOCK))
		type = 's';
	return (type);
}

char	*get_file_permissions(struct stat s)
{
	static char			bits[11];
	static const char	*rwx[] = {
		"---", "--x", "-w-", "-wx",
		"r--", "r-x", "rw-", "rwx"
	};

	bits[0] = get_file_type(s);
	ft_strcpy(&bits[1], rwx[(s.st_mode >> 6) & 7]);
	ft_strcpy(&bits[4], rwx[(s.st_mode >> 3) & 7]);
	ft_strcpy(&bits[7], rwx[(s.st_mode & 7)]);
	if (s.st_mode & S_ISUID)
		bits[3] = (s.st_mode & S_IXUSR) ? 's' : 'S';
	if (s.st_mode & S_ISGID)
		bits[6] = (s.st_mode & S_IXGRP) ? 's' : 'l';
	if (s.st_mode & S_ISVTX)
		bits[9] = (s.st_mode & S_IXOTH) ? 't' : 'T';
	bits[10] = '\0';
	return (bits);
}

char	*get_file_size(t_file *file, t_flags *flags)
{
	static char buff[64] = {0};
	static char *units[] = {"", "B", "K", "M", "G", "T", "E", "Z"};
	double		size;
	size_t		unit;

	ft_bzero(buff, 64);
	if (FILE_TYPE(file->st, S_IFCHR))
		return (buff);
	unit = 0;
	size = file->st.st_size;
	if (flags->human_size)
	{
		unit++;
		while (size / 1024 >= 1)
		{
			size /= 1024;
			unit++;
		}
		ft_snprintf(buff, 64, "%.1lf", size);
	}
	else
		ft_snprintf(buff, 64, "%lld", file->st.st_size);
	ft_snprintf(buff, 64, "%s%*s%s", g_found_chr_dev ? "       " : "",
				g_size_width + 1, buff, units[unit]);
	return (buff);
}

char	*get_file_datetime(t_file *file, t_flags *flags)
{
	static char buff[13];
	time_t		now;
	time_t		t;
	char		*stime;
	int			lastchange;

	ft_bzero(buff, 13);
	now = time(NULL);
	t = flags->sort_acc_time ? file->st.st_atime : file->st.st_mtime;
	stime = ctime(&t);
	lastchange = 11;
	if (now - t >= SIXMONTHS)
		lastchange += 8;
	ft_snprintf(buff, 13, "%.7s%.5s", stime + 4, stime + lastchange);
	return (buff);
}

char	*get_char_dev(t_file *file)
{
	static char buff[64];

	ft_bzero(buff, 64);
	if (FILE_TYPE(file->st, S_IFCHR))
		ft_snprintf(buff, 64, "%3d, %3d ", major(file->st.st_rdev),
					minor(file->st.st_rdev));
	return (buff);
}
