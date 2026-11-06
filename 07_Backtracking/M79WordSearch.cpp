// 给定一个 m × n 的字符网格 board 和一个字符串 word，判断 word 是否存在于网格中。
// 单词必须由相邻单元格的字母按顺序构成，相邻指上下左右四个方向。同一个单元格在一个单词中不能被重复使用。
// 示例：
// board = [["A","B","C","E"],
//          ["S","F","C","S"],
//          ["A","D","E","E"]]
// word = "ABCCED" → true（A→B→C→C→E→D）
// word = "SEE"    → true（S→E→E）
// word = "ABCB"   → false（B 不能重复用）
// 约束：
// 1 <= m, n <= 6
// 1 <= word.length <= 15
// board 和 word 仅包含大小写英文字母
// 核心思路
// 从每个格子出发，尝试用回溯匹配 word 的每个字符。走过的格子临时标记一下，防止重复使用，回退时恢复。
// 和 LC 46 全排列的思路一样——递归 + 撤销。区别是这里在二维网格上往四个方向走，而不是在数组里选数。
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

bool exist(vector<vector<char>>& board, string word)
{
    int m = board.size();
    int n = board[0].size();
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (backtrack(board, word, i, j, 0))
                return true;    
        }
    }
    return false;
}
bool backtrack(vector<vector<char>>& board, string& word, int r, int c, int index)
{
    if (index == word.size()) return true;

    if (r < 0 || r >= board.size() || c < 0 || c >= board[0].size() || board[r][c] != word[index])
        return false;

    char tmp = board[r][c];
    board[r][c] = '#'; // Mark as visited

    bool found = backtrack(board, word, r + 1, c, index + 1) ||
                 backtrack(board, word, r - 1, c, index + 1) ||
                 backtrack(board, word, r, c + 1, index + 1) ||
                 backtrack(board, word, r, c - 1, index + 1); //用或，只要有任何一个方向返回true就停止递归
    board[r][c] = tmp; //一样的套路，尝试-状态复原(这里数据结构不一样不能用pop_back了)
    return found;
}