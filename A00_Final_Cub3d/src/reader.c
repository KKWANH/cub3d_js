/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimkwanho <kimkwanho@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 12:21:03 by kimkwanho         #+#    #+#             */
/*   Updated: 2021/02/25 20:05:06 by kimkwanho        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

char				*ft_cub_reader(char *fil, t_cub3d *cub)
{
	int				fd;
	int				rst_gnl;
	char			*lin;
	char			*map;

	if ((fd = open(fil, O_RDONLY)) == -1)
		ft_tool_exit(1, cub, 1, "Cannot read the file.");
	map = (char *)malloc(sizeof(char) * 2);
	ft_util_bzero(map, sizeof(char));
	while ((rst_gnl = get_next_line(fd, &lin)) != -1)
	{
		if (lin && !ft_cub_read(lin, &map, rst_gnl, cub))
			return (0);
		free(lin);
		lin = NULL;
		if (rst_gnl == 0)
			break ;
	}
	return (map);
}

int					ft_cub_read(char *lin, char **map, int rst_gnl,
	t_cub3d *cub)
{
	int				idx;
	int				rst_sub;

	idx = 0;
	while (ft_util_is_empty(lin[idx]) == 1)
		++idx;
	if (cub->sig == 1 && lin[idx] != '\0')
		ft_tool_exit(1, cub, 1, "something problem with map description!");
	if (!(rst_sub = ft_cub_read_sub(lin, idx, cub)))
		return (0);
	if (rst_sub == 2)
	{
		if (ft_cub_read_map(lin, map, idx, rst_gnl))
		{
			cub->opt = 1;
			return (1);
		}
		else
			ft_cub_read_other(lin, idx, cub);
	}
	return (1);
}

int					ft_cub_read_sub(char *lin, char idx, t_cub3d *cub)
{
	t_num			*n;
	int				rst;

	if (!(n = (t_num *)malloc(sizeof(t_num))))
		return (0);
	ft_util_bzero(n, sizeof(t_num));
	rst = 2;
	if (lin[idx] && lin[idx + 1])
	{
		n->n1 = lin[idx];
		n->n2 = lin[idx + 1];
		if (n->n1 == 'R')
			rst = ft_cub_read_window_size(lin, cub);
		else if (n->n1 == 'N' || n->n1 == 'W' || n->n1 == 'E' ||
			n->n1 == 'S' || n->n1 == 'P' || n->n1 == 'L')
			rst = ft_cub_read_path(lin, n, idx, cub);
		else if (n->n1 == 'F' || n->n1 == 'C')
			rst = ft_cub_read_color(lin, lin[idx], idx, cub);
	}
	free(n);
	return (rst);
}

int					ft_cub_read_map(char *l, char **map, int i, int rst_gnl)
{
	if (l[i] == '1' || l[i] == '2' || l[i] == '3' || l[i] == '0' ||
		l[i] == 'N' || l[i] == 'S' || l[i] == 'W' || l[i] == 'E')
	{
		*map = ft_util_strjoin(*map, l);
		if (rst_gnl != 0 && l[i])
			*map = ft_util_strjoin(*map, "\n");
		return (1);
	}
	return (0);
}

int					ft_cub_read_other(char *lin, int idx, t_cub3d *cub)
{
	if (lin[idx])
	{
		if (lin[idx] != '\0' && lin[idx] != '\n')
		{
			ft_tool_exit(1, cub, 1,
				ft_util_strjoin_nofree("unknown identifier : ", lin));
		}
	}
	else if (lin[idx] == '\0' && cub->opt == 1)
		cub->sig = 1;
	return (1);
}
