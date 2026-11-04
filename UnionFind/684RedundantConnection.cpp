// 给定一棵树（无向无环连通图）额外加了一条边，变成了 n 个节点 n 条边的图。以二维数组 edges 给出，
// 其中 edges[i] = [u, v] 表示节点 u 和 v 之间的一条边。
// 找出那条多余的边，删掉它能让图重新变成一棵树。如果有多个答案，返回 edges 中最后出现的那条。

// 逐条加边，加到某条边时发现两个端点已经连通了，说明这条边是多余的——加上它就成环了。
// 怎么快速判断两个节点是否已经连通？并查集 (Union-Find)。
#include <random>
#include <vector>
using namespace std;

class Solution
{
public:
    vector<int> parent;
    vector<int> rank;

    int find(int x)
    {
        if (parent[x] != x)
        {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    bool unite(int x, int y)
    {
        int px = find(x);
        int py = find(y);
        if (px == py) return false;
        //rank 用来做按秩合并（union by rank）：合并两棵树时，把矮的那棵挂到高的那棵下面，让树尽量保持矮，这样 find 往上找根时要走的步数就少。
        if (rank[px] < rank[py]) swap(px, py);
        parent[py] = px;
        if (rank[px] == rank[py]) rank[px]++;
        //因为两棵等高的树合并后，新树会高一层。所以这行++
        return true;
    }

    vector<int> findRedundantConnection(vector<vector<int>>& edges)
    {
        int n = edges.size();
        parent.resize(n + 1);
        rank.resize(n + 1, 0);
        for (int i = 1; i <= n; ++i)
        {
            parent[i] = i; //初始化每个节点是自己的parent
        }
        for (auto& e : edges)
        {
            if (!unite(e[0], e[1]))
            {
                return e; //如果两个节点已经连通，说明这条边是多余的
            }   
        }
        return {};
    }
};
