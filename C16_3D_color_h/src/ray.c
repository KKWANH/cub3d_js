#include			"ray.h"

ray_t				rays[NUM_RAYS];

void				normalizeAngle(float *angle)
{
	*angle = remainder(*angle, 2*PI);
	if (angle < 0)
		*angle = (2 * PI) + *angle;
}

float				distancePoints(float x1, float y1, float x2, float y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

void				castRay(float rayAngle, int stripId)
{ //TODO: All the crazy logic for horz, vert, ...
	normalizeAngle(&rayAngle);

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
	while (isInsideMap(nextHorzTouchX, nextHorzTouchY))
	{
		float		xToCheck = nextHorzTouchX;
		float		yToCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);

		if (hasWallAt(xToCheck, yToCheck) == true)
		{// Found a wall hit
			horzWallHitX	 = nextHorzTouchX;
			horzWallHitY	 = nextHorzTouchY;
			horzWallContent	 = getMapAt((int)floor(yToCheck/TILE_SIZE), (int)floor(xToCheck/TILE_SIZE)) ;
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
	while (isInsideMap(nextVertTouchX, nextVertTouchY))
	{
		float		xToCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
		float		yToCheck = nextVertTouchY;

		if (hasWallAt(xToCheck, yToCheck) == true)
		{// Found a wall hit
			vertWallHitX	 = nextVertTouchX;
			vertWallHitY	 = nextVertTouchY;
			vertWallContent	 = getMapAt((int)floor(yToCheck/TILE_SIZE), (int)floor(xToCheck/TILE_SIZE));
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

void				renderRays(void)
{
	for (int i=0; i<NUM_RAYS; i+=50)
	{
		drawLine(
			MINIMAP_SCALE_FACTOR * player.x,
			MINIMAP_SCALE_FACTOR * player.y,
			MINIMAP_SCALE_FACTOR * rays[i].wallHitX,
			MINIMAP_SCALE_FACTOR * rays[i].wallHitY,
			0xFF0000FF
		);
	}
}