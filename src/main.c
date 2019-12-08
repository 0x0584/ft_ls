/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 00:23:22 by archid-           #+#    #+#             */
/*   Updated: 2019/12/09 00:40:04 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int main(int argc, char *argv[])
{
	DIR *repo;					/* the actual directory */
	struct dirent *content;		/* content node */
	int i;

	if (argc < 2)
		return (-1);
	i = 1;
	while (i < argc)
	{
		if ((repo = opendir(argv[i])))
		{
			ft_printf("\n%s:\n", argv[i]);
			while ((content = readdir(repo)))
				printf("%llu %s\n", content->d_ino, content->d_name);
			closedir(repo);
		}
		else
			ft_printf("\ncannot read %s\n", argv[i]);
		i++;
	}
    return 0;
}
