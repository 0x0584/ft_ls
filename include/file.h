/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 21:06:59 by archid-           #+#    #+#             */
/*   Updated: 2020/01/26 18:53:36 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_H
# define FILE_H

# include "common.h"

# define FILE_TYPE(s, type)	  ((s.st_mode & S_IFMT) == type)
# define DOT_REPOS(path)	  (!ft_strcmp(".", path) || !ft_strcmp("..", path))
# define SIXMONTHS			  ((365 / 2) * 86400)

struct					s_file
{
	char			*path;
	char			*color;
	char			*name;

	struct stat		st;
	bool			islnk;
	char			*pwd;
	char			*grp;
};

bool					file_init(t_file *file, const char *path,
									const char *name, bool get_link_info);
void					file_del(t_file **f);
void					queue_file_del(void *e, size_t size);

char					*read_link_name(t_file *file);

char					get_file_type(struct stat s);
char					*get_full_path(t_file *file);
char					*get_file_name(t_file *file);
char					*get_file_permissions(struct stat s);
char					*get_file_size(t_file *file, t_flags *flags);
char					*get_file_datetime(t_file *file, t_flags *flags);
char					*get_file_xattr(t_file *file);
char					*get_char_dev(t_file *file);

#endif
