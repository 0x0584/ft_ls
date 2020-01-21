/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 21:07:43 by archid-           #+#    #+#             */
/*   Updated: 2020/01/22 00:17:30 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
# define COMMON_H

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
/* getpwuid */
#include <pwd.h>
/* getgrgid */
#include <grp.h>
/* acl_get_link_np, acl_get_entery */
#include <sys/acl.h>
/* listxattr */
#include <sys/xattr.h>

# include <sys/types.h>
/* major, minor */
#include <sys/ioctl.h>

extern int				g_link_width;
extern int				g_size_width;
extern int				g_uid_width;
extern int				g_gid_width;
extern uid_t			uid;
extern struct passwd	*pw;

#endif /* COMMON_H */
