// 将 n 个皇后放在 n × n 的棋盘上，使得任何两个皇后都不能互相攻击。皇后可以攻击同一行、同一列、
// 同一对角线上的棋子。
// 返回所有不同的解法，每个解法用字符串数组表示棋盘，'Q' 表示皇后，'.' 表示空位。
// 示例：
// 输入：n = 4
// 输出：
//  . Q . .      . . Q .
//  . . . Q      Q . . .
//  Q . . .      . . . Q
//  . . Q .      . Q . .

// 约束：
// 1 <= n <= 9
// 思路：逐行放皇后，每行放一个（因为每行只能有一个皇后）。
// 对于每行的每一列，检查放在这里是否和之前的皇后冲突。冲突条件是同列或同对角线。
// 关键观察——如何快速判断对角线冲突：
// 棋盘坐标 (row, col)
// 主对角线 (\)：同一条上 row - col 相同
// 副对角线 (/)：同一条上 row + col 相同
// 例如 n = 4：
// row-col:          row+col:
//  0 -1 -2 -3       0  1  2  3
//  1  0 -1 -2       1  2  3  4
//  2  1  0 -1       2  3  4  5
//  3  2  1  0       3  4  5  6
// 用三个set分别记录哪些列、哪些主对角线、哪些副对角线已被占用。
#include <algorithm>
#include <unordered_set>
#include <vector>
#include <string>
using namespace std;

vector<vector<string>> solveNQueens(int n) {
    vector<vector<string>> result;
    vector<string> board(n, string(n, '.'));
    unordered_set<int> cols; // 记录哪些列被占用
    unordered_set<int> diag1; // 记录哪些主对角线被占用 (row - col)
    unordered_set<int> diag2; // 记录哪些副对角线被占用 (row + col)
    backtrack(n, 0, baord, cols, diag1, diag2, result);
    return result;
}

void backtrack(int n, int row, vector<string>& board, unordered_set<int>& cols,
    unordered_set<int>& diag1, unordered_set<int>& diag2, vector<vector<string>>& result) {
    if (row == n)
    {
        result.push_back(board);
        return;
    }
    for (int col = 0;col < n; col++)
    {
        if (cols.contains(col) || diag1.contains(row - col) || diag2.contains(row + col))
            continue; // 冲突，跳过
        // 放置皇后
        board[row][col] = 'Q';
        cols.insert(col);
        diag1.insert(row - col);
        diag2.insert(row + col);
        
        backtrack(n, row + 1, board, cols, diag1, diag2, result);
        
        // 回溯
        board[row][col] = '.';
        cols.erase(col);
        diag1.erase(row - col);
        diag2.erase(row + col);
    }
}