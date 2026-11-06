// 给定一个 9 × 9 的数独棋盘，其中空格用 '.' 表示。填入数字 '1' 到 '9'，使得：
// 每一行的数字不重复;每一列的数字不重复;每一个 3×3 小宫格的数字不重复
// 保证输入有且仅有唯一解。
// 示例：
// 输入：
// 5 3 .  . 7 .  . . .
// 6 . .  1 9 5  . . .
// . 9 8  . . .  . 6 .
//
// 8 . .  . 6 .  . . 3
// 4 . .  8 . 3  . . 1
// 7 . .  . 2 .  . . 6
//
// . 6 .  . . .  2 8 .
// . . .  4 1 9  . . 5
// . . .  . 8 .  . 7 9
//
// 输出：
// 5 3 4  6 7 8  9 1 2
// 6 7 2  1 9 5  3 4 8
// 1 9 8  3 4 2  5 6 7
//
// 8 5 9  7 6 1  4 2 3
// 4 2 6  8 5 3  7 9 1
// 7 1 3  9 2 4  8 5 6
//
// 9 6 1  5 3 7  2 8 4
// 2 8 7  4 1 9  6 3 5
// 3 4 5  2 8 6  1 7 9
// 核心思路:回溯。找到一个空格，尝试填 '1' 到 '9'，如果不冲突就填进去继续递归，冲突了就撤销换下一个数。
// 和 LC 46、39、79 同一个套路：选择 → 递归 → 撤销。只不过这里的"选择"是往空格里填数字，"合法性"是行、列、宫格都不能重复。
#include <algorithm>
#include <unordered_map>
#include <vector>
using namespace std;

void solveSudoku(vector<vector<char>>& board)
{
    backtrack(board);
}

bool backtrack(vector<vector<char>>& board)
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (board[i][j] == '.')
            {
                // 尝试填 1~9
                for (char c = '1'; c <= '9'; ++c)
                {
                    if (isValid(board, i, j, c))
                    {
                        board[i][j] = c;
                        if (backtrack(board))
                            return true;
                        board[i][j] = '.'; // 撤销
                    }
                }
                return false; // 没有合法的数字可以填
            }
        }
    }
    return true; // 填完了
}

bool isValid(vector<vector<char>>& board, int row, int col, char c) {
    for (int i = 0; i < 9; ++i) {
        // 检查行
        if (board[row][i] == c) return false;

        // 检查列
        if (board[i][col] == c) return false;

        // 检查 3×3 宫格
        int r = 3 * (row / 3) + i / 3;
        int cc = 3 * (col / 3) + i % 3;
        if (board[r][cc] == c) return false;
    }
    return true;
}
