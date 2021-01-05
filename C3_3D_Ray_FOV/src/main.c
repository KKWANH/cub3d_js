#include			"constants.h"
#include			<stdio.h>

const	int			map[MAP_NUM_ROWS][MAP_NUM_COLS] =
{
		{ 1, 1, 1, 1, 1,  1, 1, 1, 1, 1,  1, 1, 1 ,1, 1,  1, 1, 1, 1, 1},
		{ 1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1},
		{ 1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1},
		{ 1, 0, 0, 0, 1,  0, 1, 0, 1, 0,  1, 0, 1, 0, 1,  0, 1, 0, 0, 1},
		{ 1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1},

		{ 1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 1, 0, 0, 1},
		{ 1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 1, 0, 0, 1},
		{ 1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 1, 0, 0, 1},
		{ 1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 1, 1,  1, 1, 0, 0, 1},
		{ 1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1},

		{ 1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1},
		{ 1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1},
		{ 1, 1, 1, 1, 1,  1, 1, 1, 1, 1,  1, 1, 1, 1, 1,  1, 1, 1, 1, 1}
};

struct				Player
{
	float			x;
	float			y;
	float			width;
	float			height;
	int				turnDirection;	// -1 for left, +1 for right
	int				walkDirection;	// -1 for back, +1 for front
	float			rotationAngle;
	float			walkSpeed;
	float			turnSpeed;
}					player;

SDL_Window			*window				= NULL;
SDL_Renderer		*renderer			= NULL;
int					isGameRunning		= FALSE;
int					ticksLastFrame;

int					initializeWindow(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{ fprintf(stderr, "Error: initializing SDL\n");			return	(FALSE);}

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, 				WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);
	if (!window)
	{ fprintf(stderr, "Error: creating SDL window\n");		return	(FALSE); }

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{ fprintf(stderr, "Error: creating SDL renderer\n");	return	(FALSE); }

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
	return	(TRUE);
}

void				setup(void)
{
	player.x				= WINDOW_WIDTH / 2;
    player.y				= WINDOW_HEIGHT / 2;
    player.width			= 1;
    player.height			= 1;
    player.turnDirection	= 0;
    player.walkDirection	= 0;
    player.rotationAngle	= PI / 2;
    player.walkSpeed		= 100;
    player.turnSpeed		= 45 * (PI / 180);
}

void				processInput(void)
{
	SDL_Event		event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
		case SDL_QUIT:
		{
			isGameRunning = FALSE;
			break;
		}
		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
				isGameRunning = FALSE;
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

int					hasWallAt(float posX, float posY)
{
	int				mapIdxX;
	int				mapIdxY;

	if (posX < 0 || posX > WINDOW_WIDTH || posY < 0 || posY > WINDOW_HEIGHT)
		return		(TRUE);
	mapIdxX = floor((double)posX / TILE_SIZE);
	mapIdxY = floor((double)posY / TILE_SIZE);
	if (map[mapIdxY][mapIdxX] != 0)
		return (TRUE);
	return (FALSE);
}

// TODO: move my player
void				movePlayer(float deltaTime)
{
	player.rotationAngle += player.turnDirection * player.turnSpeed * deltaTime;

	float			moveStep 	= player.walkDirection * player.walkSpeed * deltaTime;
	float			newPlayerX	= player.x + cos(player.rotationAngle) * moveStep;
	float			newPlayerY	= player.y + sin(player.rotationAngle) * moveStep;

	// TODO: perform wall collision
	// printf("hasWallAt[%d, %d]? -> ", (int)newPlayerX, (int)newPlayerY);
	if (hasWallAt(newPlayerX, newPlayerY) == FALSE)
	{
		player.x = newPlayerX;
		player.y = newPlayerY;
	}
}

void				update(void)
{
	// waste some time until we reach the target frame time length
	// int				timeToWait = FRAME_TIME_LENGTH - (SDL_GetTicks() - ticksLastFrame);
	// if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH)
	// 	SDL_Delay(timeToWait);
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TIME_LENGTH));
	float			deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
	ticksLastFrame = SDL_GetTicks();

	// TODO: remember to update game objects as a function of deltaTime
	movePlayer(deltaTime);
}

void				renderMap(void)
{
	int				tileX;
	int				tileY;
	int				tileColor;
	for (int i=0; i<MAP_NUM_ROWS; i++)
	{
		for (int j=0; j<MAP_NUM_COLS; j++)
		{
			tileX		= j * TILE_SIZE;
			tileY		= i * TILE_SIZE;
			tileColor	= map[i][j] != 0 ? 255 : 0;

			SDL_SetRenderDrawColor(renderer, tileColor, tileColor, tileColor, 255);
			SDL_Rect mapTileRect = {
				tileX	  * MINIMAP_SCALE_FACTOR,
				tileY	  * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR,
			};
			SDL_RenderFillRect(renderer, &mapTileRect);
		}
	}
}

void				renderPlayer(void)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Rect		playerRect = {
		player.x		* MINIMAP_SCALE_FACTOR,
		player.y		* MINIMAP_SCALE_FACTOR,
		player.width	* MINIMAP_SCALE_FACTOR,
		player.height	* MINIMAP_SCALE_FACTOR
	};
	SDL_RenderFillRect(renderer, &playerRect);
	SDL_RenderDrawLine(
		renderer,
		MINIMAP_SCALE_FACTOR * player.x,
		MINIMAP_SCALE_FACTOR * player.y,
		MINIMAP_SCALE_FACTOR * player.x + cos(player.rotationAngle) * 40,
		MINIMAP_SCALE_FACTOR * player.y + sin(player.rotationAngle) * 40
	);
}

void				render(void)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	renderMap();
	renderPlayer();

	SDL_RenderPresent(renderer);
}

void				destroyWindow(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int					main(void)
{
	isGameRunning = initializeWindow();
	setup();
	while (isGameRunning)
	{
		processInput();
		update();
		render();
	}
	destroyWindow();
	return (0);
}