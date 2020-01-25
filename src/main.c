/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 00:23:22 by archid-           #+#    #+#             */
/*   Updated: 2020/01/26 00:57:31 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "display.h"

int		g_link_width = 1;
int		g_size_width = 1;
int		g_column_width = 1;
int		g_uid_width = 1;
int		g_gid_width = 1;

void	set_flag(char flag, t_flags *flags)
{
	if (flag == FLAG_LIST)
		flags->list = true;
	else if (flag == FLAG_ONE_PER_LINE)
		flags->one_per_line = true;
	else if (flag == FLAG_HUMAN_SIZE)
		flags->human_size = true;
	else if (flag == FLAG_SHOW_ALL)
		flags->show_all = true;
	else if (flag == FLAG_SHOW_ALMOST_ALL)
		flags->show_almost = true;
	else if (flag == FLAG_RECURSIVE)
		flags->recursive = true;
	else if (flag == FLAG_SORT_REV)
		flags->sort_rev = true;
	else if (flag == FLAG_SORT_ACC_TIME)
		flags->sort_acc_time = true;
	else if (flag == FLAG_SORT_MOD_TIME)
		flags->sort_mod_time = true;
}

int		parse_flags(int ac, char **av, t_flags *flags)
{
	int i;
	int j;

	if (!flags)
		return (1);
	i = 1;
	ft_bzero(flags, sizeof(t_flags));
	while (i < ac)
	{
		j = 0;
		if (av[i][j] != '-')
			break ;
		else if (!ft_strcmp(av[i], "--"))
			return (i + 1);
		while (av[i][j++])
			set_flag(av[i][j], flags);
		i++;
	}
	return (i);
}

bool	user_has_permission(const char *path, struct stat st)
{
	char *tmp;

	if (st.st_mode & S_IRGRP)
		return (true);
	tmp = ft_strrchr(path, '/');
	ft_printf("ls: %s: Permission denied\n", tmp ? tmp + 1 : path);
	return (false);
}

void	raise_error(t_queue **files, const char *path)
{
	if (path)
		perror(path);
	queue_del(files, queue_file_del);
}

void	set_widths(struct stat st, const char *name)
{
	g_column_width = MAX((int)ft_strlen(name) + 1, g_column_width);
	g_link_width = MAX(g_link_width, ft_digitcount(st.st_nlink));
	g_size_width = MAX(g_size_width, ft_digitcount(st.st_size));
}

void	ft_ls_dir(struct stat st, t_queue **files,
					const char *path, t_flags *flags)
{
	DIR				*repo;
	struct dirent	*node;
	t_file			file;

	if (flags->recursive && ft_strcmp(path, "."))
		ft_printf("\n%s:\n", path);
	if (!user_has_permission(path, st))
		return (raise_error(files, NULL));
	repo = opendir(path);
	while ((node = readdir(repo)))
	{
		if (!(flags->show_all || flags->show_almost) && node->d_name[0] == '.')
			continue ;
		if (flags->show_almost && (!ft_strcmp(".", node->d_name)
									|| !ft_strcmp("..", node->d_name)))
			continue;
		else if (!file_init(&file, path, node->d_name, false))
			return (raise_error(files, file.name));
		queue_enq(*files, queue_node(&file, sizeof(t_file)));
		set_widths(file.st, node->d_name);
	}
	closedir(repo);
}

void	ft_ls(const char *path, t_flags *flags)
{
	struct stat		st;
	t_file			file;
	t_queue			*files;

	files = queue_init();
	if (lstat(path, &st) == -1)
		return (raise_error(&files, path));
	if (!flags->list && stat(path, &st) == -1)
		return (raise_error(&files, path));
	g_link_width = 1;
	g_size_width = 1;
	g_column_width = 1;
	if (FILE_TYPE(st, S_IFDIR))
	{
		ft_ls_dir(st, &files, path, flags);
		if (files == NULL)
			return ;
	}
	else if (!file_init(&file, path, NULL, !flags->list))
		return (raise_error(&files, path));
	else
		queue_enq(files, queue_node(&file, sizeof(t_file)));
	display_files(&files, flags);
}

bool	set_stat(const char *path, struct stat *st, t_flags *flags, bool *error)
{
	if (lstat(path, st) == -1)
	{
		perror(path);
		*error = true;
		return (false);
	}
	if (!flags->list && stat(path, st) == -1)
	{
		perror(path);
		*error = true;
		return (false);
	}
	return (true);
}

void	handle_files(t_queue *files, t_queue *repos, t_flags *flags, bool flag)
{
	t_qnode *e;

	ft_putstr(!QUEUE_ISEMPTY(files) && flag ? "\n" : "");
	flag = !QUEUE_ISEMPTY(files) || queue_size(repos) > 1;
	while (!QUEUE_ISEMPTY(files))
	{
		ft_ls((e = queue_deq(files))->blob, flags);
		queue_node_del(&e, queue_del_helper);
	}
	while (!QUEUE_ISEMPTY(repos))
	{
		e = queue_deq(repos);
		if (flag && !flags->recursive)
			ft_printf("\n%s:\n", e->blob);
		ft_ls(e->blob, flags);
		queue_node_del(&e, queue_del_helper);
	}
}

bool	prepare_args(int argc, char **argv, t_flags *flags)
{
	struct stat st;
	int			i;
	t_queue		*files;
	t_queue		*repos;
	bool		flag;

	flag = false;
	if ((i = parse_flags(argc, argv, flags)) == argc)
		return (false);
	files = queue_init();
	repos = queue_init();
	while (i < argc)
	{
		if (set_stat(argv[i], &st, flags, &flag))
			queue_enq(FILE_TYPE(st, S_IFDIR) ? repos : files,
						queue_dry_node(ft_strdup(argv[i]), sizeof(char *)));
		i++;
	}
	handle_sort(&repos, flags);
	handle_sort(&files, flags);
	handle_files(files, repos, flags, flag);
	queue_del(&repos, queue_del_helper);
	queue_del(&files, queue_del_helper);
	return (true);
}

int		main(int argc, char *argv[])
{
	t_flags flags;

	if (!prepare_args(argc, argv, &flags))
		ft_ls(".", &flags);
	return (0);
}
