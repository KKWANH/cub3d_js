/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimkwanho <kimkwanho@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 10:41:08 by kimkwanho         #+#    #+#             */
/*   Updated: 2021/02/17 00:24:38 by kimkwanho        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int					ft_event_key_press(int key, t_cub3d *cub)
{
	if (key == ESC)
		ft_tool_exit(0, cub, 0, "");
	else if (key == W && cub->key->w == 0)
		cub->key->w = 1;
	else if (key == S && cub->key->s == 0)
		cub->key->s = 1;
	else if (key == A && cub->key->a == 0)
		cub->key->a = 1;
	else if (key == D && cub->key->d == 0)
		cub->key->d = 1;
	else if (key == LEFT && cub->key->left == 0)
		cub->key->left = 1;
	else if (key == RIGHT && cub->key->right == 0)
		cub->key->right = 1;
	else if (key == UP && cub->key->up == 0)
		cub->key->up = 1;
	else if (key == DOWN && cub->key->down == 0)
		cub->key->down = 1;
	else if (key == PLS)
		ft_health_change(cub, key, 1);
	else if (key == QUES)
		cub->key->help = (cub->key->help == 0) ? 1 : 0;
	else if (key == SPACE || key == SHIFT)
		ft_event_key_press_opt(cub, key);
	return (1);
}

int					ft_event_key_press_opt(t_cub3d *cub, int opt)
{
	if (opt == SPACE &&
		cub->map->map[(int)cub->pos->poy][(int)cub->pos->pox] == '3' &&
		cub->is_game_playing == 1)
	{
		cub->is_game_playing = 2;
		cub->idx = 0;
	}
	if (opt == SHIFT && cub->is_game_playing == 0)
		cub->is_game_playing = 1;
	else if (opt == SHIFT && cub->is_game_playing == 3)
		cub->is_game_playing = 4;
	else if (opt == SHIFT && cub->is_game_playing == 4)
		cub->is_game_playing = 5;
	return (1);
}

int					ft_event_key_release(int key, t_cub3d *cub)
{
	if (key == W && cub->key->w == 1)
		cub->key->w = 0;
	else if (key == S && cub->key->s == 1)
		cub->key->s = 0;
	else if (key == A && cub->key->a == 1)
		cub->key->a = 0;
	else if (key == D && cub->key->d == 1)
		cub->key->d = 0;
	else if (key == LEFT && cub->key->left == 1)
		cub->key->left = 0;
	else if (key == RIGHT && cub->key->right == 1)
		cub->key->right = 0;
	else if (key == UP && cub->key->up == 1)
		cub->key->up = 0;
	else if (key == DOWN && cub->key->down == 1)
		cub->key->down = 0;
	return (1);
}

int					ft_event_destroy(t_cub3d *cub)
{
	ft_tool_exit(0, cub, 0, "");
	return (1);
}
