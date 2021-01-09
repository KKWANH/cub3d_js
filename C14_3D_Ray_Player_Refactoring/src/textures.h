#ifndef				TEXTURES_H
#define				TEXTURES_H

# include			<stdio.h>
# include			"upng.h"
# include			"defs.h"
# include			<stdint.h>

typedef				struct textures_h
{
	upng_t			*upngTexture;
	int				width;
	int				height;
	uint32_t		*texture_buffer;
}					texture_t;

struct textures_h	wallTextures[NUM_TEXTURES];

void				loadWallTextures();
void				freeWallTextures();

#endif