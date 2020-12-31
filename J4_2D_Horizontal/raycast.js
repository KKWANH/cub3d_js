const       TILE_SIZE           = 32;
const       MAP_NUM_ROWS        = 11;
const       MAP_NUM_COLS        = 15;

const       WINDOW_WIDTH        = MAP_NUM_COLS * TILE_SIZE; // how many columns
const       WINDOW_HEIGHT       = MAP_NUM_ROWS * TILE_SIZE; // how many rows

const       FOV_ANGLE           = 60 * (Math.PI / 180);

const       WALL_STRIP_WIDTH    = 30;
const       NUM_RAYS            = WINDOW_WIDTH / WALL_STRIP_WIDTH;

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
        if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
            return (true);
        var mapGridIndexX = Math.floor(x / TILE_SIZE);
        var mapGridIndexY = Math.floor(y / TILE_SIZE);

        return (this.grid[mapGridIndexY][mapGridIndexX] != 0);
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
                rect(tileX, tileY, TILE_SIZE, TILE_SIZE);
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
        this.moveSpeed      = 2.0;                    // moveSpeed 움직이는 속도
        this.rotationSpeed  = 0.5 * (Math.PI / 100);  // rotationSpeed : 3도씩 회전
    }

    // TODO: update player position based on turnDirection and walkDirection
    update()
    {
        this.rotationAngle += this.turnDirection * this.rotationSpeed;

        var moveStep = this.walkDirection * this.moveSpeed;
        var newPlayerX = this.x + Math.cos(this.rotationAngle) * moveStep;
        var newPlayerY = this.y + Math.sin(this.rotationAngle) * moveStep;

        // 벽이 아닐 경우 (충돌하지 않을 경우) 에만 플레이어 놓기
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
        circle(this.x, this.y, this.radius);
        // 라인
        // stroke("red");
        // line(
        //     this.x,
        //     this.y,
        //     this.x + Math.cos(this.rotationAngle) * 30,
        //     this.y + Math.sin(this.rotationAngle) * 30
        // );
    }
}

class       Ray
{
    constructor(rayAngle)
    {
        this.rayAngle = normalizeAngle(rayAngle);
        this.wallHitX = 0;
        this.wallHitY = 0;
        this.distance = 0;

        this.isRayFacingDown  = this.rayAngle > 0 && this.rayAngle < Math.PI;
        this.isRayFacingUp    = !this.isRayFacingDown;

        this.isRayFacingRight = this.rayAngle < 0.5 * Math.PI || this.rayAngle > 1.5 * Math.PI;
        this.isRayFacingLeft  = !this.isRayFacingRight;
    }

    ///////////////////////////////////////////
    // HORIZONTAL RAY_GRID INTERSECTION CODE //
    ///////////////////////////////////////////
    cast(columnId)
    {
        var xintercept, yintercept;
        var xstep, ystep;

        var foundHorzWallHit = false;
        var wallHitX = 0;
        var wallHitY = 0;

        // console.log("isfaceingright", this.isRayFacingRight);

        // find the coordinate of the closest horizontal grid intersection
        yintercept =  Math.floor(player.y / TILE_SIZE) * TILE_SIZE;
        yintercept += this.isRayFacingDown ? TILE_SIZE : 0;

        xintercept =  player.x + (yintercept - player.y) / Math.tan(this.rayAngle);

        // calculate the increment xstep and ystep
        ystep =  TILE_SIZE;
        ystep *= this.isRayFacingUp ? -1 : 1;

        xstep =  TILE_SIZE / Math.tan(this.rayAngle);
        xstep *= (this.isRayFacingLeft  && xstep > 0) ? -1 : 1;
        xstep *= (this.isRayFacingRight && xstep < 0) ? -1 : 1;

        var nextHorzTouchX = xintercept;
        var nextHorzTouchY = yintercept;

        if (this.isRayFacingUp)
            nextHorzTouchY--;

        while ( nextHorzTouchX >= 0 && nextHorzTouchX <= WINDOW_WIDTH &&
                nextHorzTouchY >= 0 && nextHorzTouchY <= WINDOW_HEIGHT)
        {
            if (grid.hasWallAt(nextHorzTouchX, nextHorzTouchY))
            {
                foundHorzWallHit = true;
                wallHitX = nextHorzTouchX;
                wallHitY = nextHorzTouchY;

                stroke("red");
                line(player.x, player.y, wallHitX, wallHitY);

                break;
            }
            else
            {
                nextHorzTouchX += xstep;
                nextHorzTouchY += ystep;
            }
        }
    }

    render()
    {
        stroke("rgba(255, 100, 50, 0.3)");
        line(
            player.x,
            player.y,
            player.x + Math.cos(this.rayAngle) * 30,
            player.y + Math.sin(this.rayAngle) * 30
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
    return (angle);
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

function    normalizeAngle(angle)
{
    angle = angle % (2 * Math.PI);
    if (angle < 0)
        angle = (2 * Math.PI) + angle;
    return (angle);
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
}

// TODO: render all objects frame by frame 모든 개체를 프레임 별로 렌더링
function    draw()
{
    // 언제나 업데이트 후 시작
    update();

    grid.render();
    for (ray of rays) {
        ray.render();
    }
    player.render();

    castAllRays();
}
