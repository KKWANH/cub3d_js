#include		<stdio.h>
#include		<stdint.h>
#include		<stdbool.h>
#include		<limits.h>
#include		"defs.h"
#include		"graphics.h"
#include		"textures.h"

const int		map[MAP_NUM_ROWS][MAP_NUM_COLS] =
{
				{1, 1, 1, 1, 1,  1, 1, 1, 1, 1,  1, 1, 1 ,1, 1,  1, 1, 1, 1, 1},
				{1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  1, 0, 0, 0, 1},
				{1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  8, 0, 0, 0, 1},
				{1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1},
				{1, 0, 0, 0, 2,  2, 0, 3, 0, 4,  0, 5, 0, 6, 0,  0, 0, 0, 0, 1},

				{1, 0, 0, 0, 0,  0, 0, 3, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1},
				{1, 0, 0, 0, 0,  0, 0, 3, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1},
				{1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 7, 0,  0, 0, 0, 0, 1},
				{1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 5},
				{1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 1, 0, 0,  0, 0, 0, 0, 5},

				{1, 0, 0, 0, 0,  0, 0, 0, 0, 9,  0, 0, 1, 0, 0,  0, 0, 0, 0, 5},
				{1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 1, 0, 0,  0, 0, 0, 0, 5},
				{1, 1, 1, 1, 1,  1, 1, 1, 1, 1,  1, 1, 1, 1, 5,  5, 5, 5, 5, 5}
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

struct				Ray
{
	float			rayAngle;
	float			wallHitX;		
	float			wallHitY;
	float			distance;
	bool			wasHitVertical;
	int				wallHitContent;
}					rays[NUM_RAYS];

bool				isGameRunning			= false;
int					ticksLastFrame;

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

bool				hasWallAt(float posX, float posY)
{
	int				mapIdxX;
	int				mapIdxY;

	if (posX < 0 || posX > MAP_NUM_COLS * TILE_SIZE ||
		posY < 0 || posY > MAP_NUM_ROWS * TILE_SIZE)
		return		(true);
	mapIdxX = floor(posX / TILE_SIZE);
	mapIdxY = floor(posY / TILE_SIZE);
	if (map[mapIdxY][mapIdxX] != 0)
		return (true);
	return (false);
}

float				distancePoints(float x1, float y1, float x2, float y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float				normalizeAngle(float angle)
{
	angle = remainder(angle, 2*PI);
	if (angle < 0)
		angle = (2 * PI) + angle;
	return (angle);
}

void				movePlayer(float deltaTime)
{ //TODO: move my player
	player.rotationAngle += player.turnDirection * player.turnSpeed * deltaTime;

	float			moveStep 	= player.walkDirection * player.walkSpeed * deltaTime;
	float			newPlayerX	= player.x + cos(player.rotationAngle) * moveStep;
	float			newPlayerY	= player.y + sin(player.rotationAngle) * moveStep;

	//TODO: perform wall collision
	if (hasWallAt(newPlayerX, newPlayerY) == false)
	{
		player.x = newPlayerX;
		player.y = newPlayerY;
	}
}

void				castRay(float rayAngle, int stripId)
{ //TODO: All the crazy logic for horz, vert, ...
	rayAngle = normalizeAngle(rayAngle);

	int				isRayFacingDown	 = rayAngle > 0 && rayAngle < PI;
	int				isRayFacingUp	 = !isRayFacingDown;

	int				isRayFacingRight = rayAngle < 0.5 * PI || rayAngle > 1.5 * PI;
	int				isRayFacingLeft	 = !isRayFacingRight;

	float			xintercept, yintercept;
	float			xstep, ystep;

	///////////////////////////////////////////
	///HORIZONTAL RAY_GRID INTERSECTION CODE///
	///////////////////////////////////////////
	bool			foundHorzWallHit = false;
	float			horzWallHitX	 = 0;
	float			horzWallHitY	 = 0;
	int				horzWallContent	 = 0;

	// Find the y-coordinate of the closest horizontal grid intersenction
	yintercept		=	floor(player.y / TILE_SIZE) * TILE_SIZE;
	yintercept		+=	isRayFacingDown ? TILE_SIZE : 0;

	// Find the x-coordinate of the closest horizontal grid intersection
	xintercept		=	player.x + (yintercept - player.y) / tan(rayAngle);

	// Calculate the increment xstep and ystep
	ystep			=	TILE_SIZE;
	ystep			*=	isRayFacingUp ? -1 : 1;

	xstep			=	TILE_SIZE / tan(rayAngle);
	xstep			*=	(isRayFacingLeft  && xstep > 0) ? -1 : 1;
	xstep			*=	(isRayFacingRight && xstep < 0) ? -1 : 1;

	float			nextHorzTouchX = xintercept;
	float			nextHorzTouchY = yintercept;

	// Increment xstep and ystep until we find a wall
	while (nextHorzTouchX >= 0 && nextHorzTouchX <= MAP_NUM_COLS * TILE_SIZE &&
		   nextHorzTouchY >= 0 && nextHorzTouchY <= MAP_NUM_ROWS * TILE_SIZE)
	{
		float		xToCheck = nextHorzTouchX;
		float		yToCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);

		if (hasWallAt(xToCheck, yToCheck) == true)
		{// Found a wall hit
			horzWallHitX	 = nextHorzTouchX;
			horzWallHitY	 = nextHorzTouchY;
			horzWallContent	 = map[(int)floor(yToCheck/TILE_SIZE)][(int)floor(xToCheck/TILE_SIZE)];
			foundHorzWallHit = true;
			break;
		}
		else
		{// Couldn't found a wall
			nextHorzTouchX += xstep;
			nextHorzTouchY += ystep;
		}
	}

	/////////////////////////////////////////
	///VERTICAL RAY_GRID INTERSECTION CODE///
	/////////////////////////////////////////
	bool			foundVertWallHit = false;
	float			vertWallHitX	 = 0;
	float			vertWallHitY	 = 0;
	int				vertWallContent	 = 0;

	// Find the y-coordinate of the closest horizontal grid intersenction
	xintercept		=	floor(player.x / TILE_SIZE) * TILE_SIZE;
	xintercept		+=	isRayFacingRight ? TILE_SIZE : 0;

	// Find the x-coordinate of the closest horizontal grid intersection
	yintercept		=	player.y + (xintercept - player.x) * tan(rayAngle);

	// Calculate the increment xstep and ystep
	xstep			=	TILE_SIZE;
	xstep			*=	isRayFacingLeft ? -1 : 1;

	ystep			=	TILE_SIZE * tan(rayAngle);
	ystep			*=	(isRayFacingUp   && ystep > 0) ? -1 : 1;
	ystep			*=	(isRayFacingDown && ystep < 0) ? -1 : 1;

	float			nextVertTouchX = xintercept;
	float			nextVertTouchY = yintercept;

	// Increment xstep and ystep until we find a wall
	while (nextVertTouchX >= 0 && nextVertTouchX <= MAP_NUM_COLS * TILE_SIZE &&
		   nextVertTouchY >= 0 && nextVertTouchY <= MAP_NUM_ROWS * TILE_SIZE)
	{
		float		xToCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
		float		yToCheck = nextVertTouchY;

		if (hasWallAt(xToCheck, yToCheck) == true)
		{// Found a wall hit
			vertWallHitX	 = nextVertTouchX;
			vertWallHitY	 = nextVertTouchY;
			vertWallContent	 = map[(int)floor(yToCheck/TILE_SIZE)][(int)floor(xToCheck/TILE_SIZE)];
			foundVertWallHit = true;
			break;
		}
		else
		{// Couldn't found a wall
			nextVertTouchX += xstep;
			nextVertTouchY += ystep;
		}
	}

	// Calculate both horizontal and vertical hit distances and choose the smallest one
	float			horzHitDistance = foundHorzWallHit
					? distancePoints(
						player.x,		player.y,
						horzWallHitX,	horzWallHitY)
					: FLT_MAX;
	float			vertHitDistance = foundVertWallHit
					? distancePoints(
						player.x,		player.y,
						vertWallHitX,	vertWallHitY)
					: FLT_MAX;

	if (vertHitDistance < horzHitDistance)
	{
		rays[stripId].distance		 = vertHitDistance;
		rays[stripId].wallHitX		 = vertWallHitX;
		rays[stripId].wallHitY		 = vertWallHitY;
		rays[stripId].wasHitVertical = true;
		rays[stripId].wallHitContent = vertWallContent;
		rays[stripId].rayAngle		 = rayAngle;
	}
	else
	{
		rays[stripId].distance		 = horzHitDistance;
		rays[stripId].wallHitX		 = horzWallHitX;
		rays[stripId].wallHitY		 = horzWallHitY;
		rays[stripId].wasHitVertical = false;
		rays[stripId].wallHitContent = horzWallContent;
		rays[stripId].rayAngle		 = rayAngle;
	}
}

void				castAllRays(void)
{	// start first ray subtracting half of our FOV
	float			rayAngle;
	for (int col=0; col<NUM_RAYS; col++)
	{
		rayAngle	= player.rotationAngle + atan((col - NUM_RAYS/2) / DIST_PROJ_PLANE);
		castRay(rayAngle, col);
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

void				renderMap(void)
{
	// int				tileX;
	// int				tileY;
	// int				tileColor;
	// for (int i=0; i<MAP_NUM_ROWS; i++)
	// {
	// 	for (int j=0; j<MAP_NUM_COLS; j++)
	// 	{
	// 		tileX		= j * TILE_SIZE;
	// 		tileY		= i * TILE_SIZE;
	// 		tileColor	= map[i][j] != 0 ? 255 : 0;

	// 		SDL_SetRenderDrawColor(renderer, tileColor, tileColor, tileColor, 255);
	// 		SDL_Rect mapTileRect = {
	// 			tileX	  * MINIMAP_SCALE_FACTOR,
	// 			tileY	  * MINIMAP_SCALE_FACTOR,
	// 			TILE_SIZE * MINIMAP_SCALE_FACTOR,
	// 			TILE_SIZE * MINIMAP_SCALE_FACTOR,
	// 		};
	// 		SDL_RenderFillRect(renderer, &mapTileRect);
	// 	}
	// }
}

void				renderRays(void)
{
	// SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	// for (int i=0; i<NUM_RAYS; i++)
	// {
	// 	SDL_RenderDrawLine(
	// 		renderer,
	// 		MINIMAP_SCALE_FACTOR * player.x,
	// 		MINIMAP_SCALE_FACTOR * player.y,
	// 		MINIMAP_SCALE_FACTOR * rays[i].wallHitX,
	// 		MINIMAP_SCALE_FACTOR * rays[i].wallHitY
	// 	);
	// }
}

void				renderPlayer(void)
{
	// SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	// SDL_Rect		playerRect = {
	// 	player.x		* MINIMAP_SCALE_FACTOR,
	// 	player.y		* MINIMAP_SCALE_FACTOR,
	// 	player.width	* MINIMAP_SCALE_FACTOR,
	// 	player.height	* MINIMAP_SCALE_FACTOR
	// };
	// SDL_RenderFillRect(renderer, &playerRect);
	// SDL_RenderDrawLine(
	// 	renderer,
	// 	MINIMAP_SCALE_FACTOR * player.x,
	// 	MINIMAP_SCALE_FACTOR * player.y,
	// 	MINIMAP_SCALE_FACTOR * player.x + cos(player.rotationAngle) * 40,
	// 	MINIMAP_SCALE_FACTOR * player.y + sin(player.rotationAngle) * 40
	// );
}

void				render(void)
{
	clearColorBuffer(0xFF000000);
	
	renderWallProjection();

	//renderMap();
	//renderRays();
	//renderPlayer();
	
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