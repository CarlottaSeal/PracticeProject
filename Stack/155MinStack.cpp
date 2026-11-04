#include <ppltasks.h>
#include <stack>
// 设计一个栈，除了支持常规的 push、pop、top 操作之外，还能在 O(1) 时间内获取栈中的最小元素。
// 实现 MinStack 类：
// MinStack()：初始化栈
// void push(int val)：将 val 压入栈
// void pop()：移除栈顶元素
// int top()：返回栈顶元素
// int getMin()：返回栈中最小的元素
// 所有操作都必须是 O(1)。
//
// 示例：
// MinStack s;
// s.push(-2);   // 栈：[-2]，最小值 -2
// s.push(0);    // 栈：[-2, 0]，最小值 -2
// s.push(-3);   // 栈：[-2, 0, -3]，最小值 -3
// s.getMin();   // 返回 -3
// s.pop();      // 弹出 -3，栈：[-2, 0]
// s.top();      // 返回 0
// s.getMin();   // 返回 -2（-3 已经被弹出了）
// 难点在于：pop 弹出当前最小值之后，怎么 O(1) 知道新的最小值是什么？
// 办法是用两个栈：一个正常存数据，另一个同步记录"当前状态下的最小值"。
// 每次 push 的时候，往最小栈里压入"当前值和最小栈栈顶"中更小的那个。
// pop 的时候两个栈同时弹。这样最小栈的栈顶永远是当前数据栈的最小值。
class MinStack
{
    std::stack<int> data;
    std::stack<int> minStack;

public:
    MinStack()
    {
        
    }
    void push(int val)
    {
        data.push(val);
        if (minStack.empty() || val <= minStack.top())
        {
            minStack.push(val);
        }
        else
        {
            minStack.push(minStack.top());
        }
    }
    void pop()
    {
        data.pop();
        minStack.pop();
    }
    int top()
    {
        return data.top();
    }
    int getMin()
    {
        return minStack.top();
    }
};