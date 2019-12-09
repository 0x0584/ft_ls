/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 00:06:17 by archid-           #+#    #+#             */
/*   Updated: 2019/12/09 02:58:51 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
#define FT_LS_H

#include "libft.h"
#include "queue.h"

/* opendir, readdir, closedir */
/* opendir return DIR * */
#include <dirent.h>
/* stat ,lstat */
#include <sys/stat.h>
/* ctime */
#include <time.h>
/* getcwd */
#include <unistd.h>

#define FILE_TYPE(s, type)				((s.st_mode & S_IFMT) == type)

enum e_ls_flags
{
	FLAG_LIST = 'l',
	FLAG_ONE_PER_LINE = '1',
	FLAG_SHOW_ALL = 'a',
	FLAG_RECURSIVE = 'R'
};



typedef struct s_flags t_flags;
struct s_flags
{
	bool list;
	bool one_per_line;
	bool show_all;
	bool recursive;
};

#endif
