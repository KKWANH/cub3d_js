#ifndef		CONSTANTS_H
#define		CONSTANTS_H

# include	<unistd.h>
# include	<stdio.h>
# include	<SDL2/SDL.h>
# include	<limits.h>
# include	<stdlib.h>

# define	FALSE					0
# define	TRUE					1
# define	PI						3.14159265

# define	TILE_SIZE				64
# define	MAP_NUM_ROWS			13
# define	MAP_NUM_COLS			20

# define	MINIMAP_SCALE_FACTOR	0.2

# define	WINDOW_WIDTH			(MAP_NUM_COLS * TILE_SIZE)
# define	WINDOW_HEIGHT			(MAP_NUM_ROWS * TILE_SIZE)

# define	TEXTURE_WIDTH			64
# define	TEXTURE_HEIGHT			64

# define	FOV_ANGLE				(60 * (PI / 180))
# define	NUM_RAYS				WINDOW_WIDTH

# define	FPS						50
# define	FRAME_TIME_LENGTH		(1000 / FPS)

const	int			map[MAP_NUM_ROWS][MAP_NUM_COLS] =
{
		{1, 1, 1, 1, 1,  1, 1, 1, 1, 1,  1, 1, 1 ,1, 1,  1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1},
		{1, 0, 0, 0, 1,  0, 2, 0, 3, 0,  4, 0, 5, 0, 6,  0, 7, 0, 0, 1},
		{1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1},

		{1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 8, 0, 0, 1},
		{1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 1, 0, 0, 1},
		{1, 0, 0, 0, 0,  5, 5, 0, 0, 0,  0, 0, 0, 0, 0,  0, 1, 0, 0, 1},
		{1, 0, 0, 0, 3,  0, 0, 0, 0, 0,  0, 0, 0, 3, 3,  3, 2, 0, 0, 1},
		{1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1},

		{1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1,  1, 1, 1, 1, 1,  1, 1, 1, 1, 1,  1, 1, 1, 1, 1}
};

#endif