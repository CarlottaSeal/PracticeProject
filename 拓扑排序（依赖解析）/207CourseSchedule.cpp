// 你总共需要修 numCourses 门课程，编号从 0 到 numCourses - 1。给定一个数组 prerequisites，
// 其中 prerequisites[i] = [a, b] 表示想修课程 a 必须先修课程 b。
// 判断是否可能完成所有课程的学习（即判断有没有循环依赖）。

// 这就是一个有向图判环问题。课程是节点，先修关系是有向边。如果图中有环，就没法完成所有课程。
// 经典做法：BFS 拓扑排序（Kahn 算法）
// 统计每个节点的入度（有几门先修课指向它）
// 把入度为 0 的节点（没有先修要求）全部入队
// 每次出队一个节点，表示这门课可以修了，把它指向的课的入度 -1
// 如果减完入度变成 0，入队
// 最后看处理了多少门课，等于 numCourses 就说明无环

#include <queue>
#include <vector>
using namespace std;
bool canFinish(int numCourses, vector<vector<int>>& prerequisites)
{
    vector<vector<int>> graph(numCourses);
    vector<int> inDegree(numCourses, 0);
    for (const auto& pre : prerequisites)
    {
        graph[pre[1]].push_back(pre[0]);
        inDegree[pre[0]]++;
    }
    queue<int> q;
    for (int i = 0; i < numCourses; ++i)
    {
        if (inDegree[i] == 0)
            q.push(i);
    }
    int count =0;
    while (!q.empty())
    {
        int cur = q.front();
        q.pop();
        count++;
        for (int next : graph[cur])
        {
            inDegree[next]--;
            if (inDegree[next] == 0)
                q.push(next);
        }
    }
    return count == numCourses;
}
