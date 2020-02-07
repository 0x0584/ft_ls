/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 14:39:17 by archid-           #+#    #+#             */
/*   Updated: 2020/02/07 12:27:51 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "display.h"

int		g_nl;

void	display_files_list(t_queue *files, t_queue *dirs, t_flags *flags)
{
	t_qnode *tmp;
	t_file	*file;

	ft_printf("total %zu\n", g_total);
	while (!QUEUE_ISEMPTY(files))
	{
		tmp = queue_deq(files);
		file = QNODE_AS(t_file, tmp);
		enqueue_dirs(dirs, file, flags);
		ft_printf("%s%s %*d %-*s  %-*s %s %s%s %s%s\n",
					get_file_permissions(file->st), get_file_xattr(file),
					g_link_width, file->st.st_nlink,
					g_uid_width, file->pwd,
					g_gid_width, file->grp,
					get_file_size(file, flags),
					get_char_dev(file),
					get_file_datetime(file, flags),
					get_file_name(file), read_link_name(file));
		queue_node_del(&tmp, queue_file_del);
	}
}

void	display_files_column(t_queue *files, t_queue *dirs, t_flags *flags)
{
	struct s_qarray	array;
	t_file			*file;
	size_t			i;
	size_t			index;
	size_t			base;

	i = 0;
	index = 0;
	base = 1;
	array.base = queue_as_array(files, true, &array.size);
	while (i++ < array.size)
	{
		file = QNODE_AS(t_file, array.base[index]);
		enqueue_dirs(dirs, file, flags);
		ft_printf("%s%#-*.zu", get_file_name(file),
					(int)g_column_width - (int)ft_strlen(file->name), 0);
		if (index + get_step(array.size) < array.size)
			index += get_step(array.size);
		else
		{
			index = base++;
			ft_putstr("\n");
		}
	}
	free(array.base);
}

void	display_files_oneline(t_queue *files, t_queue *dirs, t_flags *flags)
{
	t_qnode *file;

	while (!QUEUE_ISEMPTY(files))
	{
		file = queue_deq(files);
		enqueue_dirs(dirs, QNODE_AS(t_file, file), flags);
		ft_putendl(get_file_name(QNODE_AS(t_file, file)));
		queue_node_del(&file, queue_file_del);
	}
}

void	display_files(t_queue **files, t_flags *flags)
{
	t_queue		*sorted;
	t_queue		*dirs;
	t_qnode		*e;

	dirs = queue_init();
	sorted = handle_sort(files, flags);
	if (flags->list)
		display_files_list(sorted, dirs, flags);
	else if (flags->one_per_line)
		display_files_oneline(sorted, dirs, flags);
	else
		display_files_column(sorted, dirs, flags);
	queue_del(&sorted, queue_file_del);
	queue_mergesort(&dirs, queue_strcmp);
	while (flags->recursive && !QUEUE_ISEMPTY(dirs))
	{
		e = queue_deq(dirs);
		ft_ls(e->blob, flags);
		queue_node_del(&e, queue_del_helper);
	}
	queue_del(&dirs, queue_del_helper);
}
