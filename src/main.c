/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 00:23:22 by archid-           #+#    #+#             */
/*   Updated: 2019/12/09 03:23:10 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* gcc -Wall -Wextra src/\*.c -o ft_ls -Iinclude -Ilibft -Llibft -lft */

#include "ft_ls.h"

int			parse_flags(int ac, char **av, t_flags *flags)
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
		if (av[i][j] != '-' || ft_isdigit(av[i][j + 1]))
			break ;
		while (av[i][j++])
		{
			if (av[i][j] == FLAG_LIST)
				flags->list = true;
			else if (av[i][j] == FLAG_ONE_PER_LINE)
				flags->one_per_line = true;
			else if (av[i][j] == FLAG_SHOW_ALL)
				flags->show_all = true;
			else if (av[i][j] == FLAG_RECURSIVE)
				flags->recursive = true;
		}
		i++;
	}
	return (i);
}

/* FIXME: ctime() leave memory leaks */

void	get_file_info(const char *file, struct stat s)
{
	char		*type;

	type = "unkown!!?";
	if (FILE_TYPE(s, S_IFBLK))
		type = "block device!";
	else if (FILE_TYPE(s, S_IFCHR))
		type = "character device";
	else if (FILE_TYPE(s, S_IFDIR))
		type = "directory";
	else if (FILE_TYPE(s, S_IFIFO))
		type = "FIFO/pipe?!";
	else if (FILE_TYPE(s, S_IFLNK))
		type = "symbolic link";
	else if (FILE_TYPE(s, S_IFREG))
		type = "regular file (text?)";
	else if (FILE_TYPE(s, S_IFSOCK))
		type = "socket";
	ft_printf("%s: %s\ni-node: %ld\nmode: %lo link count: %ld\n"
			  "ownership: (uid: %ld, gid: %ld)\nblock size: %ld "
			  "file size: %lld allocated: %lld\n\n",
			  /* " - last change: %s - last access: %s - last modification: %s\n", */
			  file, type, (long)s.st_ino,
			  (unsigned long)s.st_mode, (unsigned long)s.st_nlink,
			  (long)s.st_uid, (long)s.st_gid,
			  (long)s.st_blksize, (long long)s.st_size, (long long)s.st_blocks
			  /* ctime(&s.st_ctime), ctime(&s.st_atime), ctime(&s.st_mtime) */
		);
}

void	ft_ls(const char *path, t_flags *flags)
{
	DIR				*repo;		/* the actual directory */
	struct dirent	*node;		/* node node */
	struct stat		s;			/* file status */
	char			*full_path;
	t_queue			*dirs;
	t_qnode			*e;

	if (stat(path, &s) == -1)
		return perror("stat");
	if (FILE_TYPE(s, S_IFDIR))
	{
		repo = opendir(path);
		ft_printf("\n%s:\n", path);
		dirs = queue_init();
		while ((node = readdir(repo)))
		{
			if (!flags->show_all && node->d_name[0] == '.')
				continue ;
			/* FIXME: many allocations, use a buffer! */
			full_path = ft_strjoin(path, "/");
			ft_strappend(&full_path, node->d_name);
			if (stat(full_path, &s) == -1)
				return perror("stat");
			/* ft_printf("\n>>>>>> full_path: %s\n", full_path); */
			if (flags->list)
				get_file_info(full_path, s);
			else
				ft_printf("%s%s", node->d_name,
							flags->one_per_line ? "\n" : " ");
			if (flags->recursive && FILE_TYPE(s, S_IFDIR))
				queue_enq(dirs,
						  queue_node_init(malloc(sizeof(struct s_queue_node)),
										  full_path, sizeof(char *)));
			else
				free(full_path);
		}
		closedir(repo);
		ft_printf(flags->one_per_line ? "" : "\n");
		while (flags->recursive && queue_size(dirs))
		{
			e = queue_deq(dirs);
			/* ft_printf("\n>>> dir: %s\n", e->blob); */
			ft_ls(e->blob, flags);
			queue_node_del(&e, lstdel_helper);
		}
		queue_del(&dirs, lstdel_helper);
		/* } */
		/* else */
		/* 	ft_printf("\ncannot read %s\n", path); */
	}
	else if (flags->list)
		get_file_info(path, s);
}

int		main(int argc, char *argv[])
{
	t_flags flags;
	int		i;
	char	cwd[512];

	if ((i = parse_flags(argc ,argv, &flags)) < argc)
		while (i < argc)
		{
			ft_ls(argv[i++], &flags);
			i++;
		}
	else
	{
		if (!(getcwd(cwd, sizeof(cwd))))
			perror("getcwd() error");
		else
			ft_ls(cwd, &flags);
	}
	return 0;
}
