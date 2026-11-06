// 给定一个整数数组 heights，其中 heights[i] 表示柱状图中第 i 根柱子的高度，每根柱子宽度为 1。
// 找出柱状图中能勾勒出的最大矩形面积。矩形的意思是每一根被覆盖的柱子都必须至少有那么高。
// 示例：
// 输入：heights = [2,1,5,6,2,3]
// 输出：10
// 解释：高度 5 和 6 那两根柱子组成的矩形，宽 2 × 高 5 = 10
//      ┌┐
//    ┌┐││
//    ││││┌┐
//  ┌┐││││││
//  ││││││││
//  ┘└┘└┘└┘└
//  2 1 5 6 2 3
#include <stack>
#include <vector>
using namespace std;
int largestRectArea(vector<int>& heights)
{
    int n = heights.size();
    stack<int> st; //存下标，栈内高度单调递增
    int result = 0; // 修正：原来未初始化

    for (int i = 0; i <= n; i++)
    {
        int cur;
        if (i == n) cur = 0;
        else cur = heights[i];
        // 为什么要这样？遍历过程中，只有遇到比栈顶矮的柱子才会触发弹出。所以遍历结束后，
        // 栈里留下的是那些右边没有遇到过比它们更矮的柱子。不管数组长什么样，这种情况都可能发生。
        // 这会把栈里剩余的柱子全部逼出来，逐个计算面积。

        while (!st.empty() && heights[st.top()] > cur)
        {
            int h = heights[st.top()];
            st.pop();

            int w;
            if (st.empty())
                w = i;
            else
            {
                w =  i - st.top() - 1;
            }
            result = max(result, w * h);  //result是“这根柱子本身”，wh是两个柱子围合
        }
        st.push(i);
    }
    return result;
}