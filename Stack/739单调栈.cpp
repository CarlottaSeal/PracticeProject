// 给定一个整数数组 temperatures，表示每天的温度。返回一个数组 answer，
// 其中 answer[i] 是指从第 i 天起，需要等几天才能遇到更高的温度。如果之后没有更高的温度，answer[i] = 0。
//
// 示例：
// 输入：temperatures = [73, 74, 75, 71, 69, 72, 76, 73]
// 输出：             [1,  1,  4,  2,  1,  1,  0,  0]
// 解释：
// 第0天 73 → 第1天 74 更高，等 1 天
// 第2天 75 → 第6天 76 更高，等 4 天
// 第6天 76 → 之后没有更高的，0
// 暴力做法是对每一天往后扫，O(n²)。用单调栈可以做到 O(n)。
// 核心思想：维护一个从栈底到栈顶递减的栈（存下标）。遍历每一天，如果当前温度比栈顶那天高，
// 说明栈顶那天找到了答案，弹出并计算天数差。
#include <algorithm>
#include <stack>
#include <unordered_map>
#include <vector>
using namespace std;

vector<int> dailyTemperatures(vector<int>&temperatures)
{
    vector<int> answer(temperatures.size(), 0);
    stack<int> stack; // 存储下标的单调栈
    for (int i = 0; i < temperatures.size(); ++i)
    {
        while (!stack.empty() && temperatures[i] > temperatures[stack.top()]) // 用while，处理栈里的东西总是要用while的
        {
            int prev = stack.top();
            stack.pop();
            answer[prev] = i - prev;
        }
        stack.push(i); // 修正：原来漏了入栈
    }
    return answer;
}
//这道题手画个图就都懂了
