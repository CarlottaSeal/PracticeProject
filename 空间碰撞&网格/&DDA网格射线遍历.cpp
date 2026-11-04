// 给定一张二维网格地图 grid，grid[y][x] == 1 表示该格是实心墙，0 表示空地。每个格子边长为 1，
// 格子 (x, y) 覆盖的世界坐标范围是 [x, x+1) × [y, y+1)。
// 给定一条射线：起点 origin（浮点世界坐标），方向 dir，最大长度 maxDist。
// 求射线碰到的第一个实心格子，返回命中信息：
// hit：是否命中
// dist：沿射线从起点到命中点的距离
// tile：命中的格子坐标
// pos：命中点的世界坐标
// normal：命中面的法线（格子边界的朝外方向，如从左边打进来是 (-1, 0)）
// 要求：只在射线跨过格子边界时检查格子，不能用固定小步长往前挪。
//
// 示例：
// grid（第一行是 y = 0）：
//   x: 0 1 2 3 4 5
// y=0  0 0 0 0 0 0
// y=1  0 0 0 0 0 1
// y=2  0 0 0 0 0 0
// y=3  0 1 0 0 1 0
// y=4  0 0 0 0 0 0
//
// 输入：origin = (2.3, 1.5), dir = (0.8, 0.6), maxDist = 10
// 输出：hit = true, dist = 2.5, tile = (4, 3), pos = (4.3, 3.0), normal = (0, -1)
// 解释：依次经过 (2,1) → (2,2) → (3,2) → (4,2) → (4,3)，进入 (4,3) 时撞墙
//
// 输入：origin = (2.3, 1.5), dir = (0.8, 0.6), maxDist = 2
// 输出：hit = false（走到 (4,2) 就到最大长度了）
//
// 输入：origin = (1.5, 3.5), dir = (1, 0), maxDist = 10
// 输出：hit = true, dist = 0, tile = (1, 3)（起点就在墙里）
//
// 约束：
// 1 <= 网格宽、高 <= 1000
// origin 一定在网格范围内
// dir 不为零向量，但不保证是单位向量；dir.x 或 dir.y 可能为 0
// 射线走出网格边界视为未命中
//
// 追问：
// 1. 射线正好穿过格子的角（同时跨过 x 和 y 边界）怎么处理？
//    答：此时 tMaxX == tMaxY。要么固定一个 tie-break（比如先走 x），保证结果 deterministic；
//        要么 x、y 同时走一步（走对角）。做碰撞检测这种需要 conservative 的场景，角上相邻的两个格子都要检查，否则墙角会漏。
// 2. 改成返回射线经过的所有格子
//    答：起点格子先放进结果，之后每走一步就 push_back 当前格子，碰到障碍或走出网格再停。主循环不变，只是不在第一次命中时 return。
// 3. 扩展到三维体素网格
//    答：多一个 z 轴：stepZ、tDeltaZ、tMaxZ。每一步在三个 tMax 里取最小的那个轴前进。这就是 Amanatides & Woo（1987）的 voxel traversal，体素游戏选方块、voxel raymarching 都用它。
// 4. 和"每次前进 0.01 检查一次"的写法比，复杂度和正确性差在哪？
//    答：复杂度：fixed-step 是 O(射线长度 / 步长)，大部分采样落在同一个格子里，是浪费；DDA 是 O(经过的格子数)，每个格子只查一次。
//        正确性：fixed-step 在步长内跨过格子角时会漏格子（tunneling），命中点和法线也只是近似值；DDA 是精确的，命中距离直接就是 tMax。
#include <vector>
#include <cmath>
#include <cfloat>
using namespace std;

struct Vec2 {
    float x, y;
};

struct IntVec2 {
    int x, y;
};

struct RayHit {
    bool hit = false;
    float dist = 0.f;
    IntVec2 tile = {-1, -1};
    Vec2 pos = {0.f, 0.f};
    IntVec2 normal = {0, 0};
};

RayHit raycastGrid(const vector<vector<int>>& grid, Vec2 origin, Vec2 dir, float maxDist)
{
    RayHit r;
    int h = grid.size();
    int w = grid[0].size();

    float len = sqrt(dir.x * dir.x + dir.y * dir.y);
    dir.x /= len;
    dir.y /= len;    //归一化，之后t就是距离

    int x = (int)floor(origin.x);
    int y = (int)floor(origin.y);
    if (grid[y][x] == 1)   // 起点就在墙里，dist = 0，没有法线
    {
        r.hit = true;
        r.tile = {x, y};
        r.pos = origin;
        return r;
    }

    // tDelta：沿该轴跨过一整格，射线要走多长
    // tMax：沿射线走多远会碰到该轴的下一条格子边界
    // 方向为 0 就永远碰不到这一轴的边界，设成 INF
    const float INF = FLT_MAX;
    int stepX, stepY;
    float tDeltaX, tDeltaY, tMaxX, tMaxY;

    if (dir.x > 0)
    {
        stepX = 1;
        tDeltaX = 1.f / dir.x;
        tMaxX = (x + 1 - origin.x) * tDeltaX;   // 往右，下一条边界是 x+1
    }
    else if (dir.x < 0)
    {
        stepX = -1;
        tDeltaX = -1.f / dir.x;
        tMaxX = (origin.x - x) * tDeltaX;       // 往左，下一条边界是 x
    }
    else
    {
        stepX = 0;
        tDeltaX = INF;
        tMaxX = INF;
    }

    if (dir.y > 0)
    {
        stepY = 1;
        tDeltaY = 1.f / dir.y;
        tMaxY = (y + 1 - origin.y) * tDeltaY;
    }
    else if (dir.y < 0)
    {
        stepY = -1;
        tDeltaY = -1.f / dir.y;
        tMaxY = (origin.y - y) * tDeltaY;
    }
    else
    {
        stepY = 0;
        tDeltaY = INF;
        tMaxY = INF;
    }

    while (true)
    {
        float t ;
        IntVec2 n;
        if (tMaxX < tMaxY)
        {
            x += stepX;
            t = tMaxX;
            tMaxX += tDeltaX;
            n = {-stepX, 0};
        }
        else
        {
            y += stepY;
            t = tMaxY;
            tMaxY += tDeltaY;
            n = {0, -stepY};
        }

        if (t > maxDist) return r;
        if (x < 0 || x >= w || y<0 || y>=h) return r;  //走出grid了
        if (grid[y][x] == 1)
        {
            r.hit = true;
            r.dist = t;
            r.tile = {x,y};
            r.pos =  {origin.x + dir.x * t, origin.y + dir.y * t};
            r.normal = n;
            return r;
        }
    }
}
