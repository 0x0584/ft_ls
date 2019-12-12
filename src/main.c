/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 00:23:22 by archid-           #+#    #+#             */
/*   Updated: 2019/12/12 19:10:57 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* gcc -Wall -Wextra src/\*.c -o ft_ls -Iinclude -Ilibft -Llibft -lft */

#include "ft_ls.h"

int		n_link_width = 1;
int		n_size_width = 1;

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
			else if (av[i][j] == FLAG_SORT_REV)
				flags->sort_rev = true;
			else if (av[i][j] == FLAG_SORT_ACC_TIME)
				flags->sort_acc_time = true;
			else if (av[i][j] == FLAG_SORT_MOD_TIME)
				flags->sort_mod_time = true;
		}
		i++;
	}
	return (i);
}

static char	get_file_type(struct stat s)
{
	if (FILE_TYPE(s, S_IFBLK))
		return 'b';
	else if (FILE_TYPE(s, S_IFCHR))
		return 'c';
	else if (FILE_TYPE(s, S_IFDIR))
		return 'd';
	else if (FILE_TYPE(s, S_IFIFO))
		return 'p';
	else if (FILE_TYPE(s, S_IFLNK))
		return 'l';
	else if (FILE_TYPE(s, S_IFREG))
		return '-';
	else if (FILE_TYPE(s, S_IFSOCK))
		return 's';
	return '?';
}

static char *list_permissions(struct stat s)
{
    static const char *rwx[] = {
		"---", "--x", "-w-", "-wx",
		"r--", "r-x", "rw-", "rwx"
	};
    static char bits[11];

    bits[0] = get_file_type(s);
    ft_strcpy(&bits[1], rwx[(s.st_mode >> 6)& 7]);
    ft_strcpy(&bits[4], rwx[(s.st_mode >> 3)& 7]);
    ft_strcpy(&bits[7], rwx[(s.st_mode & 7)]);
    if (s.st_mode & S_ISUID)
        bits[3] = (s.st_mode & S_IXUSR) ? 's' : 'S';
    if (s.st_mode & S_ISGID)
        bits[6] = (s.st_mode & S_IXGRP) ? 's' : 'l';
    if (s.st_mode & S_ISVTX)
        bits[9] = (s.st_mode & S_IXOTH) ? 't' : 'T';
    bits[10] = '\0';
    return(bits);
}

void	list_details(const char *file, struct stat s)
{
	ft_printf("%s %s\n", list_permissions(s), file);
}

void	get_file_info(const char *path, const char *file, struct stat s)
{
	char		*type;

	/* type = "unkown!!?"; */
	/* if (FILE_TYPE(s, S_IFBLK)) */
	/* 	type = "block device!"; */
	/* else if (FILE_TYPE(s, S_IFCHR)) */
	/* 	type = "character device"; */
	/* else if (FILE_TYPE(s, S_IFDIR)) */
	/* 	type = "directory"; */
	/* else if (FILE_TYPE(s, S_IFIFO)) */
	/* 	type = "FIFO/pipe?!"; */
	/* else if (FILE_TYPE(s, S_IFLNK)) */
	/* 	type = "symbolic link"; */
	/* else if (FILE_TYPE(s, S_IFREG)) */
	/* 	type = "regular file (text?)"; */
	/* else if (FILE_TYPE(s, S_IFSOCK)) */
	/* 	type = "socket"; */
	/* ft_printf("%s: %s\ni-node: %ld\nmode: %lo link count: %ld\n" */
	/* 		  "ownership: (uid: %ld, gid: %ld)\nblock size: %ld " */
	/* 		  "file size: %lld allocated: %lld\n\n" */
	/* 		  " - last change: %s - last access: %s - last modification: %s\n", */
	/* 		  file, type, (long)s.st_ino, */
	/* 		  (unsigned long)s.st_mode, (unsigned long)s.st_nlink, */
	/* 		  (long)s.st_uid, (long)s.st_gid, */
	/* 		  (long)s.st_blksize, (long long)s.st_size, (long long)s.st_blocks, */
	/* 		  ctime(&s.st_ctime), ctime(&s.st_atime), ctime(&s.st_mtime) */
	/* 	); */

	struct passwd	*pwd;
	struct group	*grp;

	pwd = getpwuid(s.st_uid);
	grp = getgrgid(s.st_gid);

	ft_printf("%s %*d %s %s %*lld %s\n",
			  list_permissions(s),
			  n_link_width,
			  s.st_nlink,
			  pwd->pw_name, grp->gr_name,
			  n_size_width,
			  s.st_size,
			  file);
}

void	display_files(t_lst *files, t_flags *flags)
{
	t_lst		walk;
	t_queue		*dirs;
	t_qnode		*e;
	t_file		*foo;
	t_lst		tmp;

	walk = handle_sort(files, flags);
	dirs = queue_init();
	while (walk)
	{
		foo = walk->content;
		if (flags->list)
			get_file_info(foo->path, foo->name, foo->st);
		else
			ft_printf("%s%s", foo->name,
					  flags->one_per_line ? "\n" : " ");
		if (ft_strcmp(".", foo->name) && ft_strcmp("..", foo->name)
				&& flags->recursive && FILE_TYPE(foo->st, S_IFDIR))
			queue_enq(dirs,
					  queue_node_init(malloc(sizeof(struct s_queue_node)),
									  foo->path, sizeof(char *)));
		else
			free(foo->path);
		free(foo->name);
		tmp = walk;
		walk = walk->next;
		free(tmp->content);
		free(tmp);
	}
	while (flags->recursive && queue_size(dirs))
	{
		e = queue_deq(dirs);
		ft_printf("\n>>> dir: %s\n", e->blob);
		ft_ls(e->blob, flags);
		queue_node_del(&e, lstdel_helper);
	}
	queue_del(&dirs, lstdel_helper);
}

void	ft_ls(const char *path, t_flags *flags)
{
	DIR				*repo;		/* the actual directory */
	struct dirent	*node;		/* node node */
	struct stat		s;
	t_file			f;
	t_lst			files;

	if (stat(path, &s) == -1)
		return perror("stat");
	files = NULL;
	n_link_width = 1;
	n_size_width = 1;
	if (FILE_TYPE(s, S_IFDIR))
	{
		repo = opendir(path);
		ft_printf("\n%s:\n", path);
		while ((node = readdir(repo)))
		{
			if (!flags->show_all && node->d_name[0] == '.')
				continue ;
			/* FIXME: many allocations, use a buffer! */
			f.path = ft_strjoin(path, "/");
			ft_strappend(&f.path, node->d_name);
			f.name = ft_strdup(node->d_name);
			if (stat(f.path, &f.st) == -1)
				return perror("stat");
			ft_lstadd(&files, ft_lstnew(&f, sizeof(t_file)));
			n_link_width = MAX(n_link_width, ft_digitcount(f.st.st_nlink));
			n_size_width = MAX(n_size_width, ft_digitcount(f.st.st_size));
		}
		closedir(repo);
	}
	else
	{
		/* f.path = ft_strjoin(path, "/"); */
		/* ft_strappend(&f.path, node->d_name); */
		f.path = ft_strdup(path);
		f.name = (char *)path;
		if (stat(f.path, &f.st) == -1)
			return perror("stat");
		ft_lstadd(&files, ft_lstnew(&f, sizeof(t_file)));
	}
	display_files(&files, flags);
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
