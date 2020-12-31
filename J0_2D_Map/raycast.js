const       TILE_SIZE       = 32;
const       MAP_NUM_ROWS    = 11;
const       MAP_NUM_COLS    = 15;

const       WINDOW_WIDTH    = MAP_NUM_COLS * TILE_SIZE; // how many columns
const       WINDOW_HEIGHT   = MAP_NUM_ROWS * TILE_SIZE; // how many rows

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

var         grid = new Map();

// TODO: initialize all objects 모든 오브젝트 초기화하기
function    setup()
{
    createCanvas(WINDOW_WIDTH, WINDOW_HEIGHT);
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
