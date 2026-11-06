// 给定一个 m × n 的二维网格 grid，其中 '1' 表示陆地，'0' 表示水。网格四个方向（上下左右）
// 相邻的陆地连成一片岛屿。计算网格中岛屿的数量。网格边界外视为水。
// 示例：
// 输入：
//   1 1 0 0 0
//   1 1 0 0 0
//   0 0 1 0 0
//   0 0 0 1 1
// 输出：3
//1. DFS
#include <queue>
#include <vector>
using namespace std;
int numIslands(vector<vector<char>>& grid) {
    if (grid.empty()) return 0;
    int m = grid.size();
    int n = grid[0].size();
    int count = 0;

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == '1')
            {
                ++count;
                dfs(grid, i, j);
            }
        }
    }
    return count;
}
void dfs(vector<vector<char>>& grid, int i, int j)
{
    int m = grid.size();
    int n = grid[0].size();
    if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] == '0') return; // 修正：原来没有越界检查

    grid[i][j] = '0';
    dfs(grid, i - 1, j);
    dfs(grid, i + 1, j);
    dfs(grid, i, j - 1);
    dfs(grid, i, j + 1);
}
//2. BFS
int numIslands(vector<vector<char>>& grid)
{
    int m = grid.size();
    int n = grid[0].size();
    int count = 0;
    int dx[] = { 0, 1, 0, -1 }; // 修改：统一成和 994、874 一样的方向顺序
    int dy[] = { 1, 0, -1, 0 };
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == '1')
            {
                ++count;
                grid[i][j] = '0';
                queue<pair<int, int>> q;
                q.push({ i, j });
                while (!q.empty())
                {
                    auto [x, y] = q.front();
                    q.pop();
                    for (int k = 0; k < 4; ++k)
                    {
                        int nx = x + dx[k];
                        int ny = y + dy[k];
                        if (nx >= 0 && nx < m && ny >= 0 && ny < n && grid[nx][ny] == '1')
                        {
                            grid[nx][ny] = '0';
                            q.push({ nx, ny });
                        }
                    }
                }
            }
        }
    }
    return count;
}
