/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 21:07:43 by archid-           #+#    #+#             */
/*   Updated: 2020/01/26 00:36:55 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
# define COMMON_H

# include "libft.h"
# include "queue.h"

# include <dirent.h>
# include <sys/stat.h>
# include <time.h>
# include <unistd.h>
# include <pwd.h>
# include <grp.h>
# include <sys/acl.h>
# include <sys/xattr.h>
# include <sys/types.h>
# include <sys/ioctl.h>

extern int				g_link_width;
extern int				g_size_width;
extern int				g_column_width;
extern int				g_uid_width;
extern int				g_gid_width;

typedef struct s_flags	t_flags;
typedef struct s_file	t_file;

#endif
