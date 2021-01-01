const       TILE_SIZE           = 64;
const       MAP_NUM_ROWS        = 11;
const       MAP_NUM_COLS        = 15;

const       WINDOW_WIDTH        = MAP_NUM_COLS * TILE_SIZE; // how many columns
const       WINDOW_HEIGHT       = MAP_NUM_ROWS * TILE_SIZE; // how many rows

const       FOV_ANGLE           = 60 * (Math.PI / 180);

const       WALL_STRIP_WIDTH    = 1;
const       NUM_RAYS            = WINDOW_WIDTH / WALL_STRIP_WIDTH;

const       MINIMAP_SCALE_FACTOR = 0.2;

class       Map
{
    constructor()
    {
        this.grid = [
            [1, 1, 1, 1, 1,  1, 1, 1, 1, 1,  1, 1, 1, 1, 1],
            [1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 1, 0, 1],
            [1, 0, 0, 0, 0,  1, 0, 0, 0, 0,  0, 0, 1, 0, 1],
            [1, 1, 1, 1, 0,  0, 0, 0, 0, 0,  1, 0, 1, 0, 1],
            [1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  1, 0, 1, 0, 1],
            [1, 0, 0, 0, 0,  0, 0, 0, 1, 1,  1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1,  1, 0, 0, 0, 1,  1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1,  1, 1, 1, 1, 1,  1, 1, 1, 1, 1]
        ];
    }

    hasWallAt(x, y)
    {
        var mapGridIndexX = Math.floor(x / TILE_SIZE);
        var mapGridIndexY = Math.floor(y / TILE_SIZE);
        return this.grid[mapGridIndexY][mapGridIndexX] != 0;
    }

    render()
    {
        for (var i=0; i<MAP_NUM_ROWS; i++)
        {
            for (var j=0; j<MAP_NUM_COLS; j++)
            {
                var tileX = j * TILE_SIZE;
                var tileY = i * TILE_SIZE;
                var tileColor = this.grid[i][j] == 1 ? "#222" : "#fff";
                stroke("#222");
                fill(tileColor);
                rect(
                    MINIMAP_SCALE_FACTOR * tileX,
                    MINIMAP_SCALE_FACTOR * tileY,
                    MINIMAP_SCALE_FACTOR * TILE_SIZE,
                    MINIMAP_SCALE_FACTOR * TILE_SIZE);
            }
        }
    }
}

class       Player
{
    constructor()
    {
        this.x              = WINDOW_WIDTH / 2;
        this.y              = WINDOW_HEIGHT / 2;
        this.radius         = 3;
        this.turnDirection  = 0;                      // turnDirection : -1(left), 1(right), 0(nothing to update) : 실제 각도가 아니라 왼쪽인지 오른쪽인지만.
        this.walkDirection  = 0;                      // walkDirection : -1(back), 1(front), 0(nothing to update) : 실제 각도가 아니라 앞인지 뒤인지만.
        this.rotationAngle  = Math.PI / 2;            // rotationAngle : PI = 180도, PI/2 = 90도s
        this.moveSpeed      = 4.0;                    // moveSpeed 움직이는 속도
        this.rotationSpeed  = 2.0 * (Math.PI / 100);  // rotationSpeed : 3도씩 회전
    }

    // TODO: update player position based on turnDirection and walkDirection
    update()
    {
        this.rotationAngle += this.turnDirection * this.rotationSpeed;

        var moveStep    = this.walkDirection * this.moveSpeed;
        var newPlayerX  = this.x + Math.cos(this.rotationAngle) * moveStep;
        var newPlayerY  = this.y + Math.sin(this.rotationAngle) * moveStep;

        // 벽이 아닐 경우(충돌하지 않을 경우)에만 플레이어 놓기
        if (!grid.hasWallAt(newPlayerX, newPlayerY))
        {
            this.x = newPlayerX;
            this.y = newPlayerY;
        }
    }

    render()
    {
        noStroke();
        // 위치
        fill("red");
        circle(
            MINIMAP_SCALE_FACTOR * this.x,
            MINIMAP_SCALE_FACTOR * this.y,
            MINIMAP_SCALE_FACTOR * this.radius);
        // 라인
        // stroke("red");
        // line(
        //     MINIMAP_SCALE_FACTOR * this.x,
        //     MINIMAP_SCALE_FACTOR * this.y,
        //     MINIMAP_SCALE_FACTOR * this.x + Math.cos(this.rotationAngle) * 30,
        //     MINIMAP_SCALE_FACTOR * this.y + Math.sin(this.rotationAngle) * 30
        // );
    }
}

class       Ray
{
    constructor(angle)
    {
        this.rayAngle           = normalizeAngle(angle);
        this.wallHitX           = 0;
        this.wallHitY           = 0;
        this.distance           = 0;
        this.wasHitVertical     = false;

        this.isRayFacingDown    = this.rayAngle > 0 && this.rayAngle < Math.PI;
        this.isRayFacingUp      = !this.isRayFacingDown;

        this.isRayFacingRight   = this.rayAngle < 0.5 * Math.PI || this.rayAngle > 1.5 * Math.PI;
        this.isRayFacingLeft    = !this.isRayFacingRight;
    }

    cast(columnId)
    {
        var xintercept, yintercept;
        var xstep, ystep;

        ///////////////////////////////////////////
        // HORIZONTAL RAY-GRID INTERSECTION CODE
        ///////////////////////////////////////////
        var foundHorzWallHit = false;
        var horzWallHitX     = 0;
        var horzWallHitY     = 0;

        // Find the y-coordinate of the closest horizontal grid intersenction
        yintercept  =  Math.floor(player.y / TILE_SIZE) * TILE_SIZE;
        yintercept  += this.isRayFacingDown ? TILE_SIZE : 0;

        // Find the x-coordinate of the closest horizontal grid intersection
        xintercept  =  player.x + (yintercept - player.y) / Math.tan(this.rayAngle);

        // Calculate the increment xstep and ystep
        ystep       =  TILE_SIZE;
        ystep       *= this.isRayFacingUp ? -1 : 1;

        xstep       =  TILE_SIZE / Math.tan(this.rayAngle);
        xstep       *= (this.isRayFacingLeft && xstep > 0) ? -1 : 1;
        xstep       *= (this.isRayFacingRight && xstep < 0) ? -1 : 1;

        var nextHorzTouchX = xintercept;
        var nextHorzTouchY = yintercept;

        // if (this.isRayFacingUp)
        //     nextHorzTouchY--;

        // Increment xstep and ystep until we find a wall
        while (nextHorzTouchX >= 0 && nextHorzTouchX <= WINDOW_WIDTH &&
                nextHorzTouchY >= 0 && nextHorzTouchY <= WINDOW_HEIGHT)
        {
            if (grid.hasWallAt(nextHorzTouchX, nextHorzTouchY - (this.isRayFacingUp ? 1 : 0)))
            {
                foundHorzWallHit = true;
                horzWallHitX = nextHorzTouchX;
                horzWallHitY = nextHorzTouchY;
                break;
            }
            else
            {
                nextHorzTouchX += xstep;
                nextHorzTouchY += ystep;
            }
        }

        ///////////////////////////////////////////
        // VERTICAL RAY-GRID INTERSECTION CODE
        ///////////////////////////////////////////
        var foundVertWallHit = false;
        var vertWallHitX     = 0;
        var vertWallHitY     = 0;

        // Find the x-coordinate of the closest vertical grid intersenction
        xintercept  =  Math.floor(player.x / TILE_SIZE) * TILE_SIZE;
        xintercept  += this.isRayFacingRight ? TILE_SIZE : 0;

        // Find the y-coordinate of the closest vertical grid intersection
        yintercept  =  player.y + (xintercept - player.x) * Math.tan(this.rayAngle);

        // Calculate the increment xstep and ystep
        xstep       =  TILE_SIZE;
        xstep       *= this.isRayFacingLeft ? -1 : 1;

        ystep       =  TILE_SIZE * Math.tan(this.rayAngle);
        ystep       *= (this.isRayFacingUp   && ystep > 0) ? -1 : 1;
        ystep       *= (this.isRayFacingDown && ystep < 0) ? -1 : 1;

        var nextVertTouchX = xintercept;
        var nextVertTouchY = yintercept;

        // Increment xstep and ystep until we find a wall
        while (nextVertTouchX >= 0 && nextVertTouchX <= WINDOW_WIDTH &&
                nextVertTouchY >= 0 && nextVertTouchY <= WINDOW_HEIGHT)
        {
            if (grid.hasWallAt(nextVertTouchX - (this.isRayFacingLeft ? 1 : 0), nextVertTouchY))
            {
                foundVertWallHit = true;
                vertWallHitX = nextVertTouchX;
                vertWallHitY = nextVertTouchY;
                break;
            }
            else
            {
                nextVertTouchX += xstep;
                nextVertTouchY += ystep;
            }
        }

        // Calculate both horizontal and vertical distances and choose the smallest value
        var horzHitDistance = (foundHorzWallHit)
            ? distanceBetweenPoints(player.x, player.y, horzWallHitX, horzWallHitY)
            : Number.MAX_VALUE;
        var vertHitDistance = (foundVertWallHit)
            ? distanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY)
            : Number.MAX_VALUE;

        // only store the smallest of the distances
        this.wallHitX       = (horzHitDistance < vertHitDistance) ? horzWallHitX : vertWallHitX;
        this.wallHitY       = (horzHitDistance < vertHitDistance) ? horzWallHitY : vertWallHitY;
        this.distance       = (horzHitDistance < vertHitDistance) ? horzHitDistance : vertHitDistance;
        this.wasHitVertical = (horzHitDistance > vertHitDistance);
    }

    render()
    {
        stroke("rgba(255, 0, 0, 1)");
        line(
            MINIMAP_SCALE_FACTOR * player.x,
            MINIMAP_SCALE_FACTOR * player.y,
            MINIMAP_SCALE_FACTOR * this.wallHitX,
            MINIMAP_SCALE_FACTOR * this.wallHitY
        );
    }
}

var         grid    = new Map();
var         player  = new Player();
var         rays    = [];

function    normalizeAngle(angle)
{
    angle = angle % (2 * Math.PI);
    if (angle < 0)
        angle = (2 * Math.PI) + angle;
    return angle;
}

// TODO :
function    keyPressed()
{
    if (keyCode == UP_ARROW) {
        player.walkDirection = +1;
    } else if (keyCode == DOWN_ARROW) {
        player.walkDirection = -1;
    } else if (keyCode == RIGHT_ARROW) {
        player.turnDirection = +1;
    } else if (keyCode == LEFT_ARROW) {
        player.turnDirection = -1;
    }
}

function    keyReleased()
{
    if (keyCode == UP_ARROW) {
        player.walkDirection = 0;
    } else if (keyCode == DOWN_ARROW) {
        player.walkDirection = 0;
    } else if (keyCode == RIGHT_ARROW) {
        player.turnDirection = 0;
    } else if (keyCode == LEFT_ARROW) {
        player.turnDirection = 0;
    }
}

function    castAllRays()
{
    var     columnId = 0;

    // start first rays subtracting half of the FOV
    var     rayAngle = player.rotationAngle - (FOV_ANGLE / 2);

    rays = [];

    // for (var i=0; i<NUM_RAYS; i++)
    for (var i=0; i<NUM_RAYS; i++)
    {
        var ray = new Ray(rayAngle);
        ray.cast(columnId);
        rays.push(ray);
        rayAngle += FOV_ANGLE / NUM_RAYS;
        columnId++;
    }
}

function    distanceBetweenPoints(x1, y1, x2, y2)
{
    return Math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

function    render3DProjectedWalls()
{
    // loop every ray in the array of rays
    for (var i=0; i<NUM_RAYS; i++)
    {
        var ray = rays[i];

        var rayDistance = rays[i].distance;

        // calculate the distance in the projection plane
        var distanceProjectionPlane = (WINDOW_WIDTH / 2) / Math.tan(FOV_ANGLE / 2);

        // projected wall height
        var wallStripHeight = (TILE_SIZE / rayDistance) * distanceProjectionPlane;

        fill("rgba(255, 255, 255, 1.0)");
        // noStroke();
        rect(
            i * WALL_STRIP_WIDTH,
            (WINDOW_HEIGHT / 2) - (wallStripHeight / 2),
            WALL_STRIP_WIDTH,
            wallStripHeight
        );
    }
}

// TODO: initialize all objects 모든 오브젝트 초기화하기
function    setup()
{
    createCanvas(WINDOW_WIDTH, WINDOW_HEIGHT);
}

// TODO: update all game objects before we render the next frame 다음 프레임을 렌더링 하기 전에 모든 게임 오브젝트를 업데이트
function    update()
{
    player.update();
    castAllRays();
}

// TODO: render all objects frame by frame 모든 개체를 프레임 별로 렌더링
function    draw()
{
    // 언제나 업데이트 후 시작
    clear("#212121");
    update();

    render3DProjectedWalls();

    grid.render();
    for (ray of rays) {
        ray.render();
    }
    player.render();
}
