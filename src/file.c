/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 01:02:15 by archid-           #+#    #+#             */
/*   Updated: 2019/12/12 18:32:34 by archid-          ###   ########.fr       */
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

t_file	file_init(char *path, char *name)
{
	t_file foo;

	foo.path = path;
	foo.name = name;
	stat(get_full_path(&foo), &foo.st);
	return (foo);
}
