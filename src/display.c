/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 14:39:17 by archid-           #+#    #+#             */
/*   Updated: 2020/01/26 00:57:00 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "display.h"

int		g_nl;

void	enqueue_dirs(t_queue *dirs, t_file *file, t_flags *flags)
{
	if (flags->recursive && FILE_TYPE(file->st, S_IFDIR)
			&& ft_strcmp(".", file->name) && ft_strcmp("..", file->name))
		queue_enq(dirs, queue_dry_node(ft_strdup(file->path), sizeof(char *)));
}

void	display_files_list(t_queue *files, t_queue *dirs, t_flags *flags)
{
	t_qnode *tmp;
	t_file	*file;

	while (!QUEUE_ISEMPTY(files))
	{
		tmp = queue_deq(files);
		file = QNODE_AS(t_file, tmp);
		enqueue_dirs(dirs, file, flags);
		ft_printf("%s%s %*d %-*s  %-*s %s %s%s %s%s\n",
					get_file_permissions(file->st), get_file_xattr(file),
					g_link_width, file->st.st_nlink,
					g_uid_width, file->pwd->pw_name,
					g_gid_width, file->grp->gr_name,
					get_file_size(file, flags),
					get_char_dev(file),
					get_file_datetime(file, flags),
					get_file_name(file), read_link_name(file));
		queue_node_del(&tmp, queue_file_del);
	}
}

size_t	get_nl(void)
{
	struct winsize	w;
	static bool		set = false;

	if (!set)
	{
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		set = true;
	}
	return (w.ws_col / (g_column_width + 1));
}

void	display_files_column(t_queue *files, t_queue *dirs, t_flags *flags)
{
	t_qnode			**array;
	size_t			size;
	size_t			i;
	size_t			index;
	size_t			base;

	i = 0;
	index = 0;
	base = 1;
	array = queue_as_array(files, true, &size);
	g_nl = get_nl();
	while (i++ < size)
	{
		enqueue_dirs(dirs, QNODE_AS(t_file, array[index]), flags);
		ft_printf("%s%#-*.zu%s", get_file_name(QNODE_AS(t_file, array[index])),
					(int)(g_column_width
					- ft_strlen(QNODE_AS(t_file, array[index])->name) - 1), 0,
					index + GET_STEP(size, g_nl) < size ? " " : "\n");
		if (index + GET_STEP(size, g_nl) < size)
			index += GET_STEP(size, g_nl);
		else
			index = base++;
	}
	free(array);
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
	while (flags->recursive && !QUEUE_ISEMPTY(dirs))
	{
		e = queue_deq(dirs);
		ft_ls(e->blob, flags);
		queue_node_del(&e, queue_del_helper);
	}
	queue_del(&dirs, queue_del_helper);
}
