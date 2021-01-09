#include			<stdio.h>
#include			<stdint.h>
#include			<stdbool.h>
#include			<limits.h>

#include			"defs.h"
#include			"graphics.h"
#include			"map.h"
#include			"ray.h"
#include			"player.h"
#include			"textures.h"

bool				isGameRunning			= false;
int					ticksLastFrame;

void				setup(void)
{

	// load textures from "textures.h"
	// asks uPng library to decode all PNG files and loads the wallTextures array
	loadWallTextures();
}

void				processInput(void)
{
	SDL_Event		event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
		case SDL_QUIT:
		{
			isGameRunning = false;
			break;
		}
		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
				isGameRunning = false;
			if (event.key.keysym.sym == SDLK_UP)
				player.walkDirection = +1;
			if (event.key.keysym.sym == SDLK_DOWN)
				player.walkDirection = -1;
			if (event.key.keysym.sym == SDLK_RIGHT)
				player.turnDirection = +1;
			if (event.key.keysym.sym == SDLK_LEFT)
				player.turnDirection = -1;
			break;
		}
		case SDL_KEYUP: {
			if (event.key.keysym.sym == SDLK_UP)
				player.walkDirection = 0;
			if (event.key.keysym.sym == SDLK_DOWN)
				player.walkDirection = 0;
			if (event.key.keysym.sym == SDLK_RIGHT)
				player.turnDirection = 0;
			if (event.key.keysym.sym == SDLK_LEFT)
				player.turnDirection = 0;
			break;
		}
	}
}

void				update(void)
{
	// waste some time until we reach the target frame time length
	int				timeToWait = FRAME_TIME_LENGTH - (SDL_GetTicks() - ticksLastFrame);
	if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH)
		SDL_Delay(timeToWait);

	float			deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
	ticksLastFrame = SDL_GetTicks();

	//TODO: remember to update game objects as a function of deltaTime
	movePlayer(deltaTime);
	castAllRays();
}

void				renderWallProjection(void)
{

	for (int x=0; x<NUM_RAYS; x++)
	{
		float			perpDistance		= rays[x].distance * cos(rays[x].rayAngle - player.rotationAngle);
		float			projectedWallHeight	= (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;
		
		int				wallStripHeight		= (int)projectedWallHeight;

		int				wallTopPixel		= (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
						wallTopPixel		= wallTopPixel < 0 ? 0 : wallTopPixel;

		int				wallBottomPixel		= (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
						wallBottomPixel		= wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

		// TODO: set the color of celling (천장) 0xFFB7FBFF
		for (int y=0; y<wallTopPixel; y++)
			drawPixel(x, y, 0xFF444444);


		// TODO: calculate textureOffsetX
		int				textureOffsetX;
		if (rays[x].wasHitVertical)
			textureOffsetX = (int)rays[x].wallHitY % TILE_SIZE;
		else
			textureOffsetX = (int)rays[x].wallHitX % TILE_SIZE;

		//get the correct texture id number from the map content
		int				texNum			= rays[x].wallHitContent - 1;
		int				texture_width	= wallTextures[texNum].width;
		int				texture_height	= wallTextures[texNum].height;

		// TODO: render the wall from wallTopPixel to wallBottomPixel : set the color of the wall based on the color from the texture
		for (int y=wallTopPixel; y<wallBottomPixel; y++)
		{
			// TODO: calculate textureOffsetY
			int				distanceFromTop	= (y + (wallStripHeight / 2)) - (WINDOW_HEIGHT / 2);
			int			textureOffsetY	= distanceFromTop * ((float)texture_height / wallStripHeight);

			uint32_t		texelColor		= wallTextures[texNum].texture_buffer[(texture_width * textureOffsetY) + textureOffsetX];
			drawPixel(x, y, texelColor);
		}

		// TODO: set the color of floor (바닥)
		for (int y=wallBottomPixel; y<WINDOW_HEIGHT; y++)
			drawPixel(x, y, 0xFF888888);
	}
}

void				render(void)
{
	clearColorBuffer(0xFF000000);
	
	
	renderWallProjection();
	// drawRect(100, 200, 500, 300, 0xFFFFFFFF);

	renderMap();
	renderRays();
	renderPlayer();
	
	renderColorBuffer();
}

void				releaseResources(void)
{
	freeWallTextures();
	destroyWindow();
}

int					main(int argc, char* argv[])
{
	isGameRunning = initializeWindow();
	setup();
	while (isGameRunning)
	{
		processInput();
		update();
		render();
	}
	releaseResources();
	return (0);
}