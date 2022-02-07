/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimkwanho <kimkwanho@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 16:28:05 by kkim              #+#    #+#             */
/*   Updated: 2021/02/20 22:54:01 by kimkwanho        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int					ft_gnl_is_nl(char *backup)
{
	int				i;

	i = 0;
	while (backup[i])
	{
		if (backup[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

int					ft_gnl_split_ln(char **backup, char **line, int cut_idx)
{
	char			*temp;
	int				len;

	(*backup)[cut_idx] = '\0';
	*line = ft_util_strdup(*backup);
	len = ft_util_strlen(*backup + cut_idx + 1);
	if (len == 0)
	{
		free(*backup);
		*backup = 0;
		return (1);
	}
	temp = ft_util_strdup(*backup + cut_idx + 1);
	free(*backup);
	*backup = temp;
	return (1);
}

int					ft_gnl_return(char **backup, char **line, int read_size)
{
	int				cut_idx;

	if (read_size < 0)
		return (-1);
	if (*backup && (cut_idx = ft_gnl_is_nl(*backup)) >= 0)
		return (ft_gnl_split_ln(backup, line, cut_idx));
	else if (*backup)
	{
		*line = *backup;
		*backup = 0;
		return (0);
	}
	*line = ft_util_strdup("");
	return (0);
}

int					get_next_line(int fd, char **line)
{
	static char		*backup[32];
	char			buf[256 + 1];
	int				read_size;
	int				cut_idx;

	if ((fd < 0) || (line == 0) || (256 <= 0))
		return (-1);
	while ((read_size = read(fd, buf, 256)) > 0)
	{
		buf[read_size] = '\0';
		backup[fd] = ft_util_strjoin(backup[fd], buf);
		if ((cut_idx = ft_gnl_is_nl(backup[fd])) >= 0)
			return (ft_gnl_split_ln(&backup[fd], line, cut_idx));
	}
	return (ft_gnl_return(&backup[fd], line, read_size));
}
