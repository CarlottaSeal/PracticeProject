// 总共 numCourses 门课，编号 0 到 numCourses - 1。给定 prerequisites，
// 其中 prerequisites[i] = [a, b] 表示修课程 a 之前必须先修课程 b。
// 返回一个可行的修课顺序。如果有多种合法顺序，返回任意一种。如果无法完成所有课程（有环），返回空数组。
// 示例：
// 输入：numCourses = 4, prerequisites = [[1,0],[2,0],[3,1],[3,2]]
// 输出：[0, 1, 2, 3] 或 [0, 2, 1, 3] 都行
// 约束：
// 1 <= numCourses <= 2000
// 0 <= prerequisites.length <= 5000
// 解题思路
// 和 207 题一模一样的 BFS 拓扑排序，唯一区别是把出队顺序记录下来就是答案。

#include <algorithm>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
    vector<int> indegree(numCourses, 0);
    vector<vector<int>> graph(numCourses);

    for (auto& pre : prerequisites)
    {
        graph[pre[1]].push_back(pre[0]);
        indegree[pre[0]]++;
    }
    queue<int> q;
    for (int i = 0; i < numCourses; ++i)
    {
        if (indegree[i] == 0)
            q.push(i);
    }
    vector<int> order;
    while (!q.empty())
    {
        int course = q.front(); //q里是可以修了的课，因此直接记录顺序即可
        q.pop();
        order.push_back(course);
        for (int nextCourse : graph[course])
        {
            indegree[nextCourse]--;
            if (indegree[nextCourse] == 0)
                q.push(nextCourse);
        }
    }
    if (order.size() == numCourses)
        return order;

    return vector<int>();  // 有环，无法完成所有课程
}