// 有 n 个城市，给定一个 n × n 的邻接矩阵 isConnected，
// 其中 isConnected[i][j] = 1 表示城市 i 和城市 j 直接相连，= 0 表示不相连。
// 省份是一组直接或间接相连的城市。返回省份的数量。
//
// 示例：
// 输入：isConnected = [[1,1,0],
//                      [1,1,0],
//                      [0,0,1]]
// 输出：2
// 城市 0 和 1 相连 → 一个省份
// 城市 2 孤立     → 一个省份
// 共 2 个省份
//
// 约束：
// 1 <= n <= 200
// isConnected[i][i] == 1（自己和自己相连）
// isConnected[i][j] == isConnected[j][i] //沿主对角线对称
//
// 解题思路
// 和 200 题（岛屿数量）本质一模一样，都是数连通分量的个数。
// 区别只是图的表示方式不同：
// 200 题：二维网格，上下左右四个方向找邻居
// 547 题：邻接矩阵，第 i 行里所有 =1 的列就是邻居
// 遍历每个城市，碰到没访问过的，省份数 +1，然后 DFS 把整个连通分量标记为已访问。
#include <algorithm>
#include <unordered_map>
#include <vector>
using namespace std;

int findCircleNum(vector<vector<int>>& isConnected) {
    int n = isConnected.size();
    vector<bool> visited(n, false);
    int provinces = 0;

    for (int i = 0; i < n; ++i) {
        //遍历是按行的。DFS 到城市 cur，就看第 cur 行，找它的邻居
        //即：固定行 cur，遍历所有列 j，isConnected[cur][j] == 1 就是邻居
        if (!visited[i]) {
            ++provinces;
            dfs(isConnected, visited, i);
        }
    }
    return provinces;
}

void dfs(vector<vector<int>>& isConnected, vector<bool>& visited, int cur)
{
    visited[cur] = true;
    for (int j = 0; j < isConnected.size(); ++j) {
        if (isConnected[cur][j] == 1 && !visited[j]) {
            dfs(isConnected, visited, j);
        }
    } 
}

//可以用并查集做