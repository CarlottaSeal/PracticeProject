// 给一个 m×n 的网格，每个格子有三种状态：0 空、1 新鲜橘子、2 腐烂橘子。
// 每一分钟，腐烂橘子会让上下左右相邻的新鲜橘子也腐烂。返回所有橘子都腐烂所需的最少分钟数，
// 如果不可能全部腐烂则返回 -1。
// 思路：多源 BFS。所有腐烂橘子同时出发，一层一层扩散。
#include <vector>
#include <queue>
using namespace std;

class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int minMinutes = 0;
        int m = grid.size();
        int n = grid[0].size();
        queue<pair<int, int>> rottenQueue;
        int freshCount = 0; // 修正：原来是 m*n - 腐烂数，把空格子也算成了新鲜橘子
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == 2) {
                    rottenQueue.push({i, j});
                }
                else if (grid[i][j] == 1) {
                    freshCount++;
                }
            }
        }

        int dx[] = {0, 1, 0, -1};
        int dy[] = {1, 0, -1, 0};
        
        while (!rottenQueue.empty())
        {
            int layerNum = rottenQueue.size();
            for (int i = 0; i < layerNum; ++i)
            {
                auto [x, y] = rottenQueue.front();
                rottenQueue.pop();
                for (int d = 0; d < 4; ++d) //direction
                {
                    int newX = x + dx[d];
                    int newY = y + dy[d];
                    if (newX >= 0 && newX < m && newY >= 0 && newY < n && grid[newX][newY] == 1)
                    {
                        grid[newX][newY] = 2;
                        rottenQueue.push({newX, newY});
                        freshCount--;
                    }
                }
            }
            minMinutes ++; //一层加一次时间，所以必须记
        }
        return freshCount==0 ? max(0, minMinutes-1) : -1; // 修正：没有橘子时 minMinutes=0，原来会返回 -1 //最后一层没有新鲜橘子被腐烂，不算时间
    }
};