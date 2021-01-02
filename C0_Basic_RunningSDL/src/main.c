#include			"constants.h"
#include			<stdio.h>

SDL_Window			*window				= NULL;
SDL_Renderer		*renderer			= NULL;
int					isGameRunning		= FALSE;
int					playerX				= 0;
int					playerY				= 0;

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
	{ fprintf(stderr, "Error: creating SDL renderer\n");		return	(FALSE); }

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
	return	(TRUE);
}

void				setup(void)
{
	// TODO: initialize game objects and variables
}

void				processInput(void)
{
	SDL_Event		event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
		case SDL_QUIT:
		{ isGameRunning = FALSE; break; }
		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
				isGameRunning = FALSE;
			break;
		}
	}
}

void				update(void)
{
	++playerX;
	++playerY;
}

void				render(void)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// TODO: render all game objects for the current frame
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_Rect		rect = {playerX, playerY, 20, 20};
	SDL_RenderFillRect(renderer, &rect);

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