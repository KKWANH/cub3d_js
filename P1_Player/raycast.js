const       TILE_SIZ    = 32;
const       MAP_ROWS    = 11;
const       MAP_COLS    = 15;

const       WIND_WID    = MAP_COLS * TILE_SIZ; // how many columns
const       WIND_HEI    = MAP_ROWS * TILE_SIZ; // how many rows

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
    x;
    y;
    // radius
    rad;
    // turnDirection : -1(left), 1(right), 0(nothing to update) : 실제 각도가 아니라 왼쪽인지 오른쪽인지만.
    dir_turn = 0;
    // walkDirection : -1(back), 1(front), 0(nothing to update) : 실제 각도가 아니라 앞인지 뒤인지만.
    dir_walk = 0;
    // rotationAngle : PI = 180도, PI/2 = 90도
    ang_rota = Math.PI / 2;
    // moveSpeed 움직이는 속도
    spd_move = 3.0;
    // rotationSpeed : 3도씩 회전
    spd_rota = 3 * (MATH.PI / 180);

}

var         grid = new Map();

// TODO: initialize all objects 모든 오브젝트 초기화하기
function    setup()
{
    createCanvas(WIND_WID, WIND_HEI);
}

// TODO: update all game objects before we render the next frame 다음 프레임을 렌더링 하기 전에 모든 게임 오브젝트를 업데이트
function    update()
{

}

// TODO: render all objects frame by frame 모든 개체를 프레임 별로 렌더링
function    draw()
{
    // 언제나 업데이트 후 시작
    update();

    grid.render();
}
