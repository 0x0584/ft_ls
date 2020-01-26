/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_extra.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/26 04:51:56 by archid-           #+#    #+#             */
/*   Updated: 2020/01/26 04:59:24 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file.h"

char	*read_link_name(t_file *file)
{
	static char buff[256] = {0};

	ft_bzero(buff, 256);
	if (file->islnk)
	{
		ft_strcpy(buff, " -> ");
		readlink(file->path, buff + ft_strlen(" -> "), file->st.st_size + 1);
	}
	return (buff);
}

char	*get_file_xattr(t_file *file)
{
	static char symb[2] = {0, 0};
	acl_entry_t tmp;
	acl_t		acl;

	*symb = ' ';
	acl = acl_get_link_np(file->path, ACL_TYPE_EXTENDED);
	if (acl && acl_get_entry(acl, ACL_FIRST_ENTRY, &tmp) == -1)
	{
		acl_free(acl);
		return (symb);
	}
	if (listxattr(file->path, NULL, 0, XATTR_NOFOLLOW) > 0)
		*symb = '@';
	else if (acl)
		*symb = '+';
	if (acl)
		acl_free(acl);
	return (symb);
}
