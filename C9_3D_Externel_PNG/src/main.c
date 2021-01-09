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
	int				wasHitVertical;
	int				isRayFacingUp;
	int				isRayFacingDown;
	int				isRayFacingRight;
	int				isRayFacingLeft;
	int				wallHitContent;
}					rays[NUM_RAYS];

SDL_Window			*window					= (void *)0;
SDL_Renderer		*renderer				= (void *)0;
int					isGameRunning			= FALSE;
int					ticksLastFrame;  

uint32_t			*colorBuffer			= (void *)0;
SDL_Texture			*colorBufferTexture		= (void *)0;

uint32_t			*wallTexture			= (void *)0;
uint32_t			*textures[NUM_TEXTURES];


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

	// allocate the total maount of bytes in memory to hold our colorbuffer
	colorBuffer = (uint32_t *)malloc(sizeof(uint32_t) * (uint32_t)WINDOW_WIDTH * (uint32_t)WINDOW_HEIGHT);

	// create SDL Texture to display the colorbuffer
	colorBufferTexture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING,
		WINDOW_WIDTH,
		WINDOW_HEIGHT
	);

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

	if (posX < 0 || posX > WINDOW_WIDTH ||
		posY < 0 || posY > WINDOW_HEIGHT)
		return		(TRUE);
	mapIdxX = floor(posX / TILE_SIZE);
	mapIdxY = floor(posY / TILE_SIZE);
	if (map[mapIdxY][mapIdxX] != 0)
		return (TRUE);
	return (FALSE);
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
	if (hasWallAt(newPlayerX, newPlayerY) == FALSE)
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
	int				foundHorzWallHit = FALSE;
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
	while (nextHorzTouchX >= 0 && nextHorzTouchX <= WINDOW_WIDTH &&
		   nextHorzTouchY >= 0 && nextHorzTouchY <= WINDOW_HEIGHT)
	{
		float		xToCheck = nextHorzTouchX;
		float		yToCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);

		if (hasWallAt(xToCheck, yToCheck) == TRUE)
		{// Found a wall hit
			horzWallHitX	 = nextHorzTouchX;
			horzWallHitY	 = nextHorzTouchY;
			horzWallContent	 = map[(int)floor(yToCheck/TILE_SIZE)][(int)floor(xToCheck/TILE_SIZE)];
			foundHorzWallHit = TRUE;
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
	int				foundVertWallHit = FALSE;
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
	while (nextVertTouchX >= 0 && nextVertTouchX <= WINDOW_WIDTH &&
		   nextVertTouchY >= 0 && nextVertTouchY <= WINDOW_HEIGHT)
	{
		float		xToCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
		float		yToCheck = nextVertTouchY;

		if (hasWallAt(xToCheck, yToCheck) == TRUE)
		{// Found a wall hit
			vertWallHitX	 = nextVertTouchX;
			vertWallHitY	 = nextVertTouchY;
			vertWallContent	 = map[(int)floor(yToCheck/TILE_SIZE)][(int)floor(xToCheck/TILE_SIZE)];
			foundVertWallHit = TRUE;
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
		rays[stripId].wasHitVertical = TRUE;
		rays[stripId].wallHitContent = vertWallContent;
	}
	else
	{
		rays[stripId].distance		 = horzHitDistance;
		rays[stripId].wallHitX		 = horzWallHitX;
		rays[stripId].wallHitY		 = horzWallHitY;
		rays[stripId].wasHitVertical = FALSE;
		rays[stripId].wallHitContent = horzWallContent;
	}
	rays[stripId].rayAngle			 = rayAngle;
	rays[stripId].isRayFacingDown	 = isRayFacingDown;
	rays[stripId].isRayFacingUp		 = isRayFacingUp;
	rays[stripId].isRayFacingRight	 = isRayFacingRight;
	rays[stripId].isRayFacingLeft	 = isRayFacingLeft;
}

void				castAllRays(void)
{	// start first ray subtracting half of our FOV
	float			rayAngle;

	rayAngle = player.rotationAngle - (FOV_ANGLE / 2);
	for (int stripId=0; stripId<NUM_RAYS; stripId++)
	{
		castRay(rayAngle, stripId);
		rayAngle += FOV_ANGLE / NUM_RAYS;
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

	//TODO: remember to update game objects as a function of deltaTime
	movePlayer(deltaTime);
	castAllRays();
}

void				generate3DProjection(void)
{
	float			distanceProjPlane;
	float			projectedWallHeight;
	float			perpDistance;
	int				wallStripHeight;
	int				wallTopPixel;
	int				wallBottomPixel;
	uint32_t		texelColor;
	int				textureOffsetX;
	int				textureOffsetY;
	int				distanceFromTop;
	int				texNum;

	for (int i=0; i<NUM_RAYS; i++)
	{
		perpDistance		= rays[i].distance * cos(rays[i].rayAngle - player.rotationAngle);
		distanceProjPlane	= (WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2);
		projectedWallHeight	= (TILE_SIZE / perpDistance) * distanceProjPlane;
		
		wallStripHeight		= (int)projectedWallHeight;

		wallTopPixel		= (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
		wallTopPixel		= wallTopPixel < 0 ? 0 : wallTopPixel;

		wallBottomPixel		= (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
		wallBottomPixel		= wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

		// TODO: set the color of celling (천장) 0xFFB7FBFF
		for (int y=0; y<wallTopPixel; y++)
			colorBuffer[(WINDOW_WIDTH * y) + i] = 0xFF444444;


		// TODO: calculate textureOffsetX
		if (rays[i].wasHitVertical)		textureOffsetX = (int)rays[i].wallHitY % TILE_SIZE;
		else							textureOffsetX = (int)rays[i].wallHitX % TILE_SIZE;

		//get the correct texture id number from the map content
		texNum = rays[i].wallHitContent - 1;

		// TODO: render the wall from wallTopPixel to wallBottomPixel : set the color of the wall based on the color from the texture
		for (int y=wallTopPixel; y<wallBottomPixel; y++)
		{
			// TODO: calculate textureOffsetY
			distanceFromTop	= (y + (wallStripHeight / 2)) - (WINDOW_HEIGHT / 2);
			textureOffsetY	= distanceFromTop * ((float)TEXTURE_HEIGHT / wallStripHeight);

			texelColor		= wallTextures[texNum].texture_buffer[(TEXTURE_WIDTH * textureOffsetY) + textureOffsetX];
			colorBuffer[(WINDOW_WIDTH * y) + i] = texelColor;
		}


		// TODO: set the color of floor (바닥)
		for (int y=wallBottomPixel; y<WINDOW_HEIGHT; y++)
			colorBuffer[(WINDOW_WIDTH * y) + i] = 0xFF888888;
		
	}
}

void				renderColorBuffer(void)
{
	SDL_UpdateTexture(
		colorBufferTexture,
		NULL,
		colorBuffer,
		(int)((uint32_t)WINDOW_WIDTH * sizeof(uint32_t))
	);
	SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
}

void				clearColorBuffer(uint32_t color)
{
	for (int x=0; x<WINDOW_WIDTH; x++)
		for (int y=0; y<WINDOW_HEIGHT; y++)
			colorBuffer[(WINDOW_WIDTH * y) + x] = color;
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

void				renderRays(void)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	for (int i=0; i<NUM_RAYS; i++)
	{
		SDL_RenderDrawLine(
			renderer,
			MINIMAP_SCALE_FACTOR * player.x,
			MINIMAP_SCALE_FACTOR * player.y,
			MINIMAP_SCALE_FACTOR * rays[i].wallHitX,
			MINIMAP_SCALE_FACTOR * rays[i].wallHitY
		);
	}
}

void				renderPlayer(void)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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
	
	generate3DProjection();

	// clear the colorbuffer
	renderColorBuffer();
	clearColorBuffer(0xFF000000);

	// display the minimap
	renderMap();
	renderRays();
	renderPlayer();

	SDL_RenderPresent(renderer);
}


void				destroyWindow(void)
{
	//TODO: free all textures
	freeWallTextures();

	SDL_DestroyTexture(colorBufferTexture);
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