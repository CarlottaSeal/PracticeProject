// 给定一个 m × n 的棋盘 board，每个格子有一个初始状态：1（活细胞）或 0（死细胞）。每个细胞与其八个相邻位置（水平、垂直、对角线）的细胞交互，遵循以下四条规则同时作用于每个细胞：
// 活细胞周围活邻居 < 2，死亡（模拟人口不足）
// 活细胞周围活邻居 2 或 3，继续存活
// 活细胞周围活邻居 > 3，死亡（模拟过度拥挤）
// 死细胞周围恰好有 3 个活邻居，复活（模拟繁殖）
// 根据以上规则，返回棋盘的下一个状态。要求原地修改棋盘（不能使用额外的棋盘）。
// 状态编码：
        //   0 → 0  (死 → 死)  保持 0
        //   1 → 1  (活 → 活)  保持 1
        //   0 → 1  (死 → 活)  标记为 2
        //   1 → 0  (活 → 死)  标记为 -1

#include <vector>
using namespace std;
// IntVec2
void gameOfLife(vector<vector<int>>& board) {
    int m = board.size();
    int n = board[0].size();

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int live = countLive(board, i, j);
            if (board[i][j] == 1)
            {
                if (live < 2 || live > 3)
                    board[i][j] = -1; // 活细胞死亡
            }
            else
            {
                if (live == 3)
                    board[i][j] = 2; // 死细胞复活
            }
        }
    }
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            board[i][j] = board[i][j] > 0 ? 1 : 0; // 恢复为原始状态
        }
    }
}

int countLive(vector<vector<int>>& board, int r, int c)
{
    int m = board.size();
    int n = board[0].size();
    int liveCount = 0;
    for (int i = r - 1; i <= r + 1; ++i)
    {
        for (int j = c - 1; j <= c + 1; ++j)
        {
            if (i == r && j == c) continue; // Skip the cell itself
            if (i >= 0 && i < m && j >= 0 && j < n) {
                // 1：原来活，下一轮还活
                // -1：原来活，下一轮要死
                if (board[i][j] == 1 || board[i][j] == -1) // Count live cells (1 or -1)
                    liveCount++;
            }
        }
    }
    return liveCount;
}