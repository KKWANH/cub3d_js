/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitmap.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimkwanho <kimkwanho@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 10:22:19 by kimkwanho         #+#    #+#             */
/*   Updated: 2021/02/25 10:34:10 by kimkwanho        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void				ft_bitmap_util(int num, unsigned char *src)
{
	src[0] = (unsigned char)(num);
	src[1] = (unsigned char)(num >> 8);
	src[2] = (unsigned char)(num >> 16);
	src[3] = (unsigned char)(num >> 24);
}

void				ft_bitmap_pxl(t_cub3d *cub, int fil)
{
	int				idx;
	int				jdx;
	int				clr;

	idx = cub->hei - 1;
	while (idx >= 0)
	{
		jdx = -1;
		while (++jdx < cub->wid)
		{
			clr = *(int *)(cub->img->dat +
			(idx * cub->img->siz + jdx * (cub->img->bpp / 8)));
			write(fil, &clr, 3);
		}
		--idx;
	}
}

void				ft_bitmap_hdr(t_cub3d *cub, int fil, int siz)
{
	unsigned char	bmp[54];

	ft_util_bzero(bmp, 54);
	bmp[0] = (unsigned char)('B');
	bmp[1] = (unsigned char)('M');
	ft_bitmap_util(siz, bmp + 2);
	bmp[10] = (unsigned char)(54);
	bmp[14] = (unsigned char)(40);
	ft_bitmap_util(cub->wid, bmp + 18);
	ft_bitmap_util(cub->hei, bmp + 22);
	bmp[26] = (unsigned char)(1);
	bmp[28] = (unsigned char)(24);
	write(fil, bmp, 54);
}

int					ft_bitmap_save(t_cub3d *cub)
{
	int				fil;
	int				siz;

	while (cub->wid % 4 != 0)
		--cub->wid;
	siz = 54 + (4 * cub->hei * cub->wid);
	fil = open("cub3D.bmp", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0666);
	if (!fil)
		ft_tool_exit(1, cub, 1, "problem with --save option");
	ft_bitmap_hdr(cub, fil, siz);
	ft_bitmap_pxl(cub, fil);
	close(fil);
	return (1);
}
