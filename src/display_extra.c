/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_extra.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/26 18:25:10 by archid-           #+#    #+#             */
/*   Updated: 2020/01/26 18:36:20 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	enqueue_dirs(t_queue *dirs, t_file *file, t_flags *flags)
{
	if (flags->recursive && FILE_TYPE(file->st, S_IFDIR)
			&& ft_strcmp(".", file->name) && ft_strcmp("..", file->name))
		queue_enq(dirs, queue_dry_node(ft_strdup(file->path), sizeof(char *)));
}

size_t	get_step(size_t size)
{
	size_t nl;

	nl = win_size.ws_col / (g_column_width + 1);
	return (size / nl) + (size % nl != 0);
}

int		queue_strcmp(t_qnode *u, t_qnode *v)
{
	if (!u || !u->blob)
		return (true);
	else if (!v || !v->blob)
		return (false);
	return (ft_strcmp(u->blob, v->blob));
}
