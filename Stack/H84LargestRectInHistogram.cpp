//给定柱状图的高度数组，求能框出的最大矩形面积。

//柱状图基本都是找左右第一个更小 -> 用单调栈
//lc网站有图，可以手画一下stack里元素的变化过程->必须用while保证单调递增
//我们处理的是i前面的那一根[否则无法找“右第一个更小”]->要遍历到n而非n-1

class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        int n = heights.size();
        int maxArea = 0;
        stack<int> stk; // 存下标，栈内高度单调递增

        for (int i = 0; i <= n; i++) {
            int curH;
            if(i == n){
                curH = 0;
            } 
            else{
                curH = heights[i];
            }
            while(!stk.empty() && heights[stk.top()] > curH){
                int h = heights[stk.top()];
                int w;
                stk.pop();
                if(!stk.empty()){
                    w = i - stk.top() -1;
                }
                else{
                    w = i;
                }
                maxArea = max(maxArea, h * w);
            }
            stk.push(i);
        }
        return maxArea;
    }
};