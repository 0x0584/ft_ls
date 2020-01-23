/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 00:23:22 by archid-           #+#    #+#             */
/*   Updated: 2020/01/23 22:18:48 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* gcc -Wall -Wextra src/\*.c -o ft_ls -Iinclude -Ilibft -Llibft -lft */

#include "ft_ls.h"

uid_t			uid;
struct passwd	*pw;

int				g_link_width = 1;
int				g_size_width = 1;
int				g_uid_width = 1;
int				g_gid_width = 1;

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
			else if (av[i][j] == FLAG_HUMAN_SIZE)
				flags->human_size = true;
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

static char *get_file_permissions(struct stat s)
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

static char		*read_link_name(t_file *file)
{
	static char buff[256] = {0};

	ft_bzero(buff, 256);
	if (file->islnk)
	{
		ft_strcpy(buff, " -> ");
		readlink(file->path, buff + ft_strlen(" -> "), file->st.st_size + 1);
	}
	else
		*buff = '\0';
	return (buff);
}

char	*get_file_size(t_file *file, t_flags *flags)
{
	static char buff[64] = {0};
	static char *units[] = {"", "B", "K", "M", "G", "T", "E", "Z"};
	double size;
	size_t unit;

	unit = 0;
	size = file->st.st_size;
	if (flags->human_size)
	{
		unit++;
		while (size / 1024 >= 1)
		{
			size /= 1024;
			unit++;
		}
		/* FIXME: find how to get the output of ls with size */
		ft_snprintf(buff, 64, "%.1lf", size);

	}
	else
	{
		ft_snprintf(buff, 64, "%lld", file->st.st_size);
	}
	ft_snprintf(buff, 64, "%*s%s", g_size_width + 1, buff, units[unit]);
	return buff;
}

char	*get_file_datetime(t_file *file, t_flags *flags)
{
	static char buff[13];
	time_t now;
	time_t t;
	char *stime;

	int lastchange;

	ft_bzero(buff, 13);
	now = time(NULL);
	t = flags->sort_acc_time ? file->st.st_atime : file->st.st_mtime;
	stime = ctime(&t);
	lastchange = 11;
	if (now - t >= SIXMONTHS)
		lastchange += 8;
	ft_snprintf(buff, 13, "%.7s%.5s", stime + 4, stime + lastchange);
	return buff;
}

char	*get_file_xattr(t_file *file)
{
	static char symb[2] = {0, 0};
	acl_t acl;
	acl_entry_t tmp;

	*symb = ' ';
	acl = acl_get_link_np(file->path, ACL_TYPE_EXTENDED);
	if (acl && acl_get_entry(acl, ACL_FIRST_ENTRY, &tmp) == -1)
	{
		acl_free(acl);
		return symb;
	}
	if (listxattr(file->path, NULL, 0, XATTR_NOFOLLOW) > 0)
		*symb = '@';
	else if (acl)
		*symb = '+';
	if (acl)
		acl_free(acl);
	return symb;
}

char *get_char_dev(t_file *file)
{
	static char buff[64];

	ft_bzero(buff, 64);
	if (FILE_TYPE(file->st, S_IFCHR))
		ft_snprintf(buff, 64, "%3d, %3d ", major(file->st.st_rdev),
					minor(file->st.st_rdev));
	return buff;
}

void	get_file_info(t_file *file, t_flags *flags)
{
	/* permissions - nlinks - user - group - file size - date - filename */
	ft_printf("%s%s %*d %-*s  %-*s %s %s%s %s%s\n",
			  get_file_permissions(file->st), get_file_xattr(file),
			  g_link_width, file->st.st_nlink,
			  g_uid_width, file->pwd->pw_name,
			  g_gid_width, file->grp->gr_name,
			  get_file_size(file, flags),
			  get_char_dev(file),
			  get_file_datetime(file, flags),
			  get_file_name(file), read_link_name(file));
}

void	display_files(t_queue **files, t_flags *flags)
{
	t_queue		*sorted;
	t_queue		*dirs;
	t_qnode		*e;
	t_qnode		*tmp;

	dirs = queue_init();
	sorted = handle_sort(files, flags);
	while (!queue_isempty(sorted))
	{
		tmp = queue_deq(sorted);
		if (flags->list)
			get_file_info(QNODE_AS(t_file, tmp), flags);
		else
			ft_printf("%s%s", get_file_name(QNODE_AS(t_file, tmp)),
					  flags->one_per_line ? "\n" : " ");
		if (flags->recursive && ft_strcmp(".", QNODE_AS(t_file, tmp)->name)
				&& ft_strcmp("..", QNODE_AS(t_file, tmp)->name)
			&& FILE_TYPE(QNODE_AS(t_file, tmp)->st, S_IFDIR))

		{
			queue_enq(dirs, queue_dry_node(QNODE_AS(t_file, tmp)->path,
										   sizeof(char *)));
			QNODE_AS(t_file, tmp)->path = NULL;
		}
		else
		{
			free(QNODE_AS(t_file, tmp)->path);
			QNODE_AS(t_file, tmp)->path = NULL;
		}
		queue_node_del(&tmp, queue_file_del);
	}
	if (!flags->list)
		ft_putchar('\n');
	while (flags->recursive && !queue_isempty(dirs))
	{
		e = queue_deq(dirs);
		ft_ls(e->blob, flags);
		queue_node_del(&e, queue_del_helper);
	}
	queue_del(&dirs, queue_del_helper);
	queue_del(&sorted, queue_del_helper);
}

bool	user_has_permission(const char *path, struct stat st)
{
	char *tmp;

	if (st.st_mode & S_IRGRP)
		return true;
	tmp = ft_strrchr(path, '/');
	ft_printf("ls: %s: Permission denied\n", tmp ? tmp + 1 : path);
	return false;
}

void	ft_ls(const char *path, t_flags *flags)
{
	DIR				*repo;		/* the actual directory */
	struct dirent	*node;		/* node node */
	struct stat		st;
	t_file			file;
	t_queue			*files;

	if (lstat(path, &st) == -1)
		return perror(path);
	if (!flags->list && stat(path, &st) == -1)
		return perror(path);
	if (!user_has_permission(path, st))
		return ;

	files = queue_init();

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

			queue_enq(files, queue_node(&file, sizeof(t_file)));

			g_link_width = MAX(g_link_width, ft_digitcount(file.st.st_nlink));
			g_size_width = MAX(g_size_width, ft_digitcount(file.st.st_size));
		}

		closedir(repo);
	}
	else if (!file_init(&file, path, path, !flags->list))
		return (perror(path));
	else
		queue_enq(files, queue_node(&file, sizeof(t_file)));
	display_files(&files, flags);
}

bool	set_stat(const char *path, struct stat *st, t_flags *flags)
{
	if (lstat(path, st) == -1)
	{
		perror(path);
		return false;
	}
    if (!flags->list && stat(path, st) == -1)
	{
		perror(path);
		return false;
	}
	return true;
}

void queue_putstr(t_qnode *e)
{
	if (!e || !e->blob)
		return;
	ft_putendl(e->blob);
}

bool	prepare_args(int argc, char **argv, t_flags *flags)
{
	int			i;
	t_qnode		*e;
	t_queue		*files;
	t_queue		*repos;
	bool		flag;
	struct stat st;

	flag = false;
	if ((i = parse_flags(argc ,argv, flags)) == argc)
		return (false);
	files = queue_init();
	repos = queue_init();
	while (i < argc)
	{
		if (set_stat(argv[i], &st, flags))
			queue_enq(FILE_TYPE(st, S_IFDIR) ? repos : files,
					  queue_dry_node(ft_strdup(argv[i]), sizeof(char *)));
		else
			flag = true;
		i++;
	}
	handle_sort(&repos, flags);
	handle_sort(&files, flags);
	ft_putstr(!queue_isempty(files) && flag ? "\n" : "");
	flag = !queue_isempty(files) || queue_size(repos) > 1;
	while (!queue_isempty(files))
	{
		ft_ls((e = queue_deq(files))->blob, flags);
		queue_node_del(&e, queue_del_helper);
	}

	while (!queue_isempty(repos))
	{
		e = queue_deq(repos);
		if (flag)
			ft_printf("\n%s:\n", e->blob);
		ft_ls(e->blob, flags);
		queue_node_del(&e, queue_dry_del_helper);
	}
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
