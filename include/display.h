/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 14:44:14 by archid-           #+#    #+#             */
/*   Updated: 2020/01/26 00:35:15 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPLAY_H
# define DISPLAY_H

# include "ft_ls.h"

# define GET_STEP(s, n)					((s / n) + ((s % n) != 0))

void	enqueue_dirs(t_queue *dirs, t_file *file, t_flags *flags);
void	display_files_list(t_queue *files, t_queue *dirs, t_flags *flags);
void	display_files_column(t_queue *files, t_queue *dirs, t_flags *flags);
void	display_files_oneline(t_queue *files, t_queue *dirs, t_flags *flags);
void	display_files(t_queue **files, t_flags *flags);

#endif
