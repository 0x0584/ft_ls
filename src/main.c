/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 00:23:22 by archid-           #+#    #+#             */
/*   Updated: 2020/01/17 22:33:38 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* gcc -Wall -Wextra src/\*.c -o ft_ls -Iinclude -Ilibft -Llibft -lft */

#include "ft_ls.h"

int		g_link_width = 1;
int		g_size_width = 1;

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
		if (av[i][j] != '-')
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
	else if (FILE_TYPE(s, S_IFLNK))
		return 'l';
	else if (FILE_TYPE(s, S_IFDIR))
		return 'd';
	else if (FILE_TYPE(s, S_IFIFO))
		return 'p';
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
    ft_strcpy(&bits[1], rwx[(s.st_mode >> 6) & 7]);
    ft_strcpy(&bits[4], rwx[(s.st_mode >> 3) & 7]);
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

void print_stat(struct stat s)
{
	char *type;

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

	ft_putendl(type);

}

char	*read_link_name(t_file *file)
{
	static char buff[256] = {0};

	if (file->islink)
	{
		ft_strcpy(buff, " -> ");
		readlink(file->name, buff + ft_strlen(" -> "), 256);
	}
	else
		*buff = '\0';
	return (buff);
}

void	get_file_info(t_file *file)
{
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

	pwd = getpwuid(file->st.st_uid);
	grp = getgrgid(file->st.st_gid);

	ft_printf("%s %*d %s %s %*lld %s%s\n",
			  list_permissions(file->st),
			  g_link_width,
			  file->st.st_nlink,
			  pwd->pw_name, grp->gr_name,
			  g_size_width,
			  file->st.st_size,
			  file->name, read_link_name(file));
}

#define QNODE_AS(type, e)							((type *)e->content)

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
		tmp = walk;
		foo = walk->content;
		walk = walk->next;

		if (flags->list)
			get_file_info(foo);
		else
			ft_printf("%s%s", foo->name,
					  flags->one_per_line ? "\n" : " ");
		if (ft_strcmp(".", foo->name) && ft_strcmp("..", foo->name)
				&& flags->recursive && FILE_TYPE(foo->st, S_IFDIR))
			queue_enq(dirs,
					  queue_node_init(malloc(sizeof(struct s_queue_node)),
									  foo->path, sizeof(char *)));
		else
			free(QNODE_AS(t_file, tmp)->path);
		free(QNODE_AS(t_file, tmp)->name);
		free(tmp->content);
		free(tmp);
	}
	if (!flags->list)
		ft_putchar('\n');
	while (flags->recursive && queue_size(dirs))
	{
		e = queue_deq(dirs);
		ft_ls(e->blob, flags);
		queue_node_del(&e, lstdel_helper);
	}
	queue_del(&dirs, lstdel_helper);
}

void	ft_ls(const char *path, t_flags *flags)
{
	DIR				*repo;		/* the actual directory */
	struct dirent	*node;		/* node node */
	struct stat		st;
	struct stat		lnk;
	t_file			file;
	t_lst			files;

	bool follow_link = true;

	if (lstat(path, &st) == -1)
		return perror(path);
	if (!flags->list)
	{
		if (stat(path, &st) == -1)
			return perror(path);
		follow_link = (lstat(path, &lnk) != -1);
	}

	/* print_stat(st); */
	/* if (!flags->list) */
	/* 	print_stat(lnk); */
	/* getchar(); */


	files = NULL;
	g_link_width = 1;
	g_size_width = 1;

	if (FILE_TYPE(st, S_IFDIR))	/* if directory */
	{
		repo = opendir(path);

		if (flags->recursive)
			ft_printf("\n%s:\n", path);

		while ((node = readdir(repo)))
		{
			if (!flags->show_all && node->d_name[0] == '.')
				continue ;
			else if (!file_init(&file, path, node->d_name, !flags->list))
				return perror(file.name);

			ft_lstadd(&files, ft_lstnew(&file, sizeof(t_file)));

			g_link_width = MAX(g_link_width, ft_digitcount(file.st.st_nlink));
			g_size_width = MAX(g_size_width, ft_digitcount(file.st.st_size));
		}

		closedir(repo);
	}
	else						/* if is file */
	{
		file_init(&file, path, path, !flags->list);
		ft_lstadd(&files, ft_lstnew(&file, sizeof(t_file)));
	}
	display_files(&files, flags);
}

int		main(int argc, char *argv[])
{
	t_flags flags;
	int		i;
	char	cwd[512];

	i = parse_flags(argc ,argv, &flags);
	if (i < argc)
		while (i < argc)
			ft_ls(argv[i++], &flags);
	else
	{
		if (!(getcwd(cwd, sizeof(cwd))))
			perror("getcwd() error");
		else
			ft_ls(cwd, &flags);
	}
	return 0;
}
