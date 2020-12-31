const       TILE_SIZ    = 32;
const       MAP_ROWS    = 11;
const       MAP_COLS    = 15;

const       WIND_WID    = MAP_COLS * TILE_SIZ; // how many columns
const       WIND_HEI    = MAP_ROWS * TILE_SIZ; // how many rows

const       ANGL_FOV    = 60 * (Math.PI / 180);

const       WALL_WID    = 5;
const       RAYS_CNT    = WIND_WID/WALL_WID;

var         x_now = 5;
var         y_now = 8;

class       Map {
    constructor()
    {
        this.grid = [
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
        ];
    }

    hasWallAt(posX, posY)
    {
        var mapIdxX = Math.floor(posX / TILE_SIZ);
        var mapIdxY = Math.floor(posY / TILE_SIZ);

        return (this.grid[mapIdxY][mapIdxX] != 0);
    }

    render()
    {
        for (var i=0; i<MAP_ROWS; i++)
        {
            for (var j=0; j<MAP_COLS; j++)
            {
                var tileX = j * TILE_SIZ;
                var tileY = i * TILE_SIZ;
                var tileColor = this.grid[i][j] == 1 ? "#222" : "#fff";
                stroke("#222");
                fill(tileColor);
                rect(tileX, tileY, TILE_SIZ, TILE_SIZ);
            }
        }
    }
}

class       Player
{
    constructor()
    {
        this.x = WIND_WID / 2;
        this.y = WIND_HEI / 2;
        this.rad = 3;
        this.dir_turn = 0;                      // turnDirection : -1(left), 1(right), 0(nothing to update) : 실제 각도가 아니라 왼쪽인지 오른쪽인지만.
        this.dir_walk = 0;                      // walkDirection : -1(back), 1(front), 0(nothing to update) : 실제 각도가 아니라 앞인지 뒤인지만.
        this.ang_rota = Math.PI / 2;            // rotationAngle : PI = 180도, PI/2 = 90도s
        this.spd_move = 2.5;                    // moveSpeed 움직이는 속도
        this.spd_rota = 3 * (Math.PI / 100);    // rotationSpeed : 3도씩 회전
    }

    // TODO: update player position based on turnDirection and walkDirection
    update()
    {
        this.ang_rota += this.dir_turn * this.spd_rota;

        var moveStep = this.dir_walk * this.spd_move;
        var newPosiX = this.x + Math.cos(this.ang_rota) * moveStep;
        var newPosiY = this.y + Math.sin(this.ang_rota) * moveStep;

        // 벽이 아닐 경우 (충돌하지 않을 경우) 에만 플레이어 놓기
        if (!grid.hasWallAt(newPosiX, newPosiY))
        {
            this.x = newPosiX;
            this.y = newPosiY;
        }
    }

    render()
    {
        noStroke();
        // 위치
        fill("red");
        circle(this.x, this.y, this.rad);
        // 라인
        stroke("red");
        line(
            this.x,
            this.y,
            this.x + Math.cos(this.ang_rota) * 30,
            this.y + Math.sin(this.ang_rota) * 30
        );
    }
}

class       Ray
{
    constructor(rayAng)
    {
        this.ray_angl = rayAng;
    }

    render()
    {
        stroke("rgba(255, 100, 50, 0.3)");
        line(
            player.x,
            player.y,
            player.x + Math.cos(this.ray_angl) * 30,
            player.y + Math.sin(this.ray_angl) * 30
        );
    }
}

var         grid    = new Map();
var         player  = new Player();
var         rays    = [];

// TODO :
function    keyPressed()
{
    if (keyCode == UP_ARROW) {
        player.dir_walk = +1;
    } else if (keyCode == DOWN_ARROW) {
        player.dir_walk = -1;
    } else if (keyCode == RIGHT_ARROW) {
        player.dir_turn = +1;
    } else if (keyCode == LEFT_ARROW) {
        player.dir_turn = -1;
    }
}

function    keyReleased()
{
    if (keyCode == UP_ARROW) {
        player.dir_walk = 0;
    } else if (keyCode == DOWN_ARROW) {
        player.dir_walk = 0;
    } else if (keyCode == RIGHT_ARROW) {
        player.dir_turn = 0;
    } else if (keyCode == LEFT_ARROW) {
        player.dir_turn = 0;
    }
}

function    castAllRays()
{
    var     colId = 0;

    // start first rays subtracting half of the FOV
    var     rayAng = player.ang_rota - (ANGL_FOV / 2);
    rays = [];

    // for (var i=0; i<RAYS_CNT; i++)
    for (var i=0; i<RAYS_CNT; i++)
    {
        rays.push(new Ray(rayAng));
        rayAng += ANGL_FOV / RAYS_CNT;
        ++colId;
    }

}

// TODO: initialize all objects 모든 오브젝트 초기화하기
function    setup()
{
    createCanvas(WIND_WID, WIND_HEI);
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
    update();

    grid.render();
    for (ray of rays)
    {
        ray.render();
    }
    player.render();
}
