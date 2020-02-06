/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 00:23:22 by archid-           #+#    #+#             */
/*   Updated: 2020/02/06 21:00:41 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

struct winsize	win_size;
int				g_link_width = 1;
int				g_size_width = 1;
int				g_column_width = 1;
int				g_uid_width = 1;
int				g_gid_width = 1;
bool			g_found_chr_dev = false;
size_t			g_total = 0;

bool	set_flag(char flag, t_flags *flags)
{
	if (!flag)
		return (true);
	else if (flag == FLAG_LIST)
		return (flags->list = true);
	else if (flag == FLAG_ONE_PER_LINE)
		return (flags->one_per_line = true);
	else if (flag == FLAG_HUMAN_SIZE)
		return (flags->human_size = true);
	else if (flag == FLAG_SHOW_ALL)
		return (flags->show_all = true);
	else if (flag == FLAG_SHOW_ALMOST_ALL)
		return (flags->show_almost = true);
	else if (flag == FLAG_RECURSIVE)
		return (flags->recursive = true);
	else if (flag == FLAG_SORT_REV)
		return (flags->sort_rev = true);
	else if (flag == FLAG_SORT_ACC_TIME)
		return (flags->sort_acc_time = true);
	else if (flag == FLAG_SORT_MOD_TIME)
		return (flags->sort_mod_time = true);
	return (false);
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
			if (!set_flag(av[i][j], flags))
				print_usage(av[i][j], av[0]);
		i++;
	}
	return (i);
}

void	handle_files(t_queue *files, t_queue *repos, t_flags *flags, bool flag)
{
	t_qnode *e;
	t_file	*file;

	ft_putstr(!QUEUE_ISEMPTY(files) && flag ? "\n" : "");
	flag = !QUEUE_ISEMPTY(files) || queue_size(repos) > 1;
	while (!QUEUE_ISEMPTY(files))
	{
		e = queue_deq(files);
		file = e->blob;
		ft_ls(file->name, flags);
		queue_node_del(&e, queue_file_del);
	}
	while (!QUEUE_ISEMPTY(repos))
	{
		e = queue_deq(repos);
		file = e->blob;
		if (flag && !flags->recursive)
			ft_printf("\n%s:\n", file->name);
		ft_ls(file->name, flags);
		queue_node_del(&e, queue_file_del);
	}
}

bool	prepare_args(int argc, char **argv, t_flags *flags)
{
	int			i;
	t_file		file;
	t_queue		*files;
	t_queue		*repos;
	bool		flag;

	flag = false;
	if ((i = parse_flags(argc, argv, flags)) == argc)
		return (false);
	files = queue_init();
	repos = queue_init();
	while (i < argc)
		if (set_stat(&file, argv[i++], flags, &flag))
			queue_enq(FILE_TYPE(file.st, S_IFDIR) ? repos : files,
						queue_node(&file, sizeof(t_file)));
	handle_sort(&repos, flags);
	handle_sort(&files, flags);
	handle_files(files, repos, flags, flag);
	queue_del(&repos, queue_file_del);
	queue_del(&files, queue_file_del);
	return (true);
}

int		main(int argc, char *argv[])
{
	t_flags flags;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_size);
	if (!prepare_args(argc, argv, &flags))
		ft_ls(".", &flags);
	return (0);
}
