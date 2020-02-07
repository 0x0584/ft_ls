/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/26 18:16:40 by archid-           #+#    #+#             */
/*   Updated: 2020/02/06 20:58:42 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	ft_ls_dir(t_queue **files, const char *path, t_flags *flags)
{
	DIR				*repo;
	struct dirent	*node;
	t_file			file;

	g_total = 0;
	if (flags->recursive && ft_strcmp(path, "."))
		ft_printf("\n%s:\n", path);
	if (!(repo = opendir(path)))
		return (raise_error(files, path));
	while ((node = readdir(repo)))
	{
		if ((!(flags->show_all || flags->show_almost) && node->d_name[0] == '.')
				|| (flags->show_almost && DOT_REPOS(node->d_name)))
			continue ;
		else if (!file_init(&file, path, node->d_name, false))
			return (raise_error(files, file.name));
		g_total += file.st.st_blocks;
		if (FILE_TYPE(file.st, S_IFCHR))
			g_found_chr_dev = true;
		queue_enq(*files, queue_node(&file, sizeof(t_file)));
		set_widths(file.st, node->d_name);
	}
	closedir(repo);
}

void		ft_ls(const char *path, t_flags *flags)
{
	struct stat		st;
	t_file			file;
	t_queue			*files;

	files = queue_init();
	if (lstat(path, &st) == -1)
		return (raise_error(&files, path));
	if (!flags->list && stat(path, &st) == -1)
		return (raise_error(&files, path));
	reset_widths();
	if (FILE_TYPE(st, S_IFDIR))
	{
		g_found_chr_dev = false;
		ft_ls_dir(&files, path, flags);
		if (files == NULL)
			return ;
	}
	else if (!file_init(&file, path, NULL, !flags->list))
		return (raise_error(&files, path));
	else
		queue_enq(files, queue_node(&file, sizeof(t_file)));
	display_files(&files, flags);
}
