/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 14:39:17 by archid-           #+#    #+#             */
/*   Updated: 2020/01/25 23:24:06 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	enqueue_dirs(t_queue *dirs, t_file *file, t_flags *flags)
{
	if (flags->recursive && FILE_TYPE(file->st, S_IFDIR)
			&& ft_strcmp(".", file->name) && ft_strcmp("..", file->name))
		queue_enq(dirs, queue_dry_node(ft_strdup(file->path), sizeof(char *)));
}

void	display_files_list(t_queue *files, t_queue *dirs, t_flags *flags)
{
	t_qnode *tmp;
	t_file *file;

	while (!queue_isempty(files))
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

static size_t get_step(size_t size, size_t nl)
{
	return (size / nl) + (size % nl != 0);
}

void	display_files_column(t_queue *files, t_queue *dirs, t_flags *flags)
{
	struct winsize	w;
    size_t			nl;
	t_qnode			**array;
	size_t			size;
	t_file			*file;
	size_t			i;
	size_t			index;
	size_t			base;


	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	nl = w.ws_col / (g_column_width + 1);

	i = 0;
	index = 0;
	base = 1;
	array = queue_as_array(files, true, &size);
	while (i < size)
	{
		i++;
		file = QNODE_AS(t_file, array[index]);
		enqueue_dirs(dirs, file, flags);
		ft_printf("%s%#-*.zu", get_file_name(file),
				  (int)(g_column_width - ft_strlen(file->name) - 1), 0);
		if (index + get_step(size, nl) < size)
		{
			index += get_step(size, nl);
			ft_putstr(" ");
		}
		else
		{
			index = base++;
			ft_putstr("\n");
		}

	}
	free(array);
}

void	display_files_oneline(t_queue *files, t_queue *dirs, t_flags *flags)
{
	t_qnode *file;

	while ( !queue_isempty(files))
	{
		file = queue_deq(files);
		enqueue_dirs(dirs, QNODE_AS(t_file, file), flags);
		ft_putendl(get_file_name(QNODE_AS(t_file, file)));
		queue_node_del(&file, queue_file_del);

	}
}

void	put_flag(bool flag)
{
	ft_printf("->  %s\n", flag ? "set" : "none");
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
	while (flags->recursive && !queue_isempty(dirs))
	{
		e = queue_deq(dirs);
		ft_ls(e->blob, flags);
		queue_node_del(&e, queue_del_helper);
	}
	queue_del(&dirs, queue_del_helper);
}
