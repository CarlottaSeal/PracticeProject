// 机器人初始位于原点 (0, 0)，面朝北方。机器人接收三种指令：
//
// -2：左转 90°
// -1：右转 90°
// 1 ~ 9：向前走对应步数（每次走一格）
//
// 同时给定一个障碍物数组 obstacles，其中每个元素是 [x, y] 坐标。机器人遇到障碍物时会停在障碍物前一格（不会穿过去）。
//
// 返回机器人在行走过程中距离原点的最大欧几里得距离的平方（即 x² + y² 的最大值）。

#include <vector>
#include <set>
using namespace std;
int robotSim(vector<int>& commands, vector<vector<int>>&obstacles)
{
    //方向数组：北、东、南、西四个方向，用 dx/dy 数组表示。左转 d = (d + 3) % 4，右转 d = (d + 1) % 4。
    int dx[] = {0, 1, 0, -1}; // 北、东、南、西
    int dy[] = {1, 0, -1, 0};

    set<pair<int, int>> obstacleSet;
    for (const auto& obs : obstacles)
    {
        obstacleSet.insert({obs[0], obs[1]});
    }
    int x = 0, y = 0, d = 0; // 初始位置和方向
    int maxDistSq = 0;
    for (int cmd : commands)
    {
        if (cmd == -2)
        {
            d = (d + 3) % 4; // 左转
        }
        else if (cmd == -1)
        {
            d = (d + 1) % 4; // 右转
        }
        else
        {
            for (int step = 0; step < cmd; ++step)
            {
                int nx = x + dx[d]; //一步一步走
                int ny = y + dy[d];
                if (!obstacleSet.contains({nx, ny}))
                {
                    x = nx;
                    y = ny;
                    maxDistSq = max(maxDistSq, x * x + y * y);
                }
                else
                {
                    break; // 遇到障碍物，停止前进
                }
            }
        }
    }
    return maxDistSq;
}
