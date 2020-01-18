/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 21:06:59 by archid-           #+#    #+#             */
/*   Updated: 2020/01/17 21:49:05 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_H
# define FILE_H

#include "common.h"

#define FILE_TYPE(s, type)				((s.st_mode & S_IFMT) == type)

typedef struct s_file t_file;
struct s_file
{
	char		*path;
	char		*name;
	struct stat st;
	struct stat lnk;
	bool		islink;
};

char *get_full_path(t_file *file);
bool file_init(t_file *file, const char *path, const char *name,
				bool get_link_info);

#endif
