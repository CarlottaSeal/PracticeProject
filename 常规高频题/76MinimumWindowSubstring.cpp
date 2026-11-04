// 给定两个字符串 s 和 t，找出 s 中包含 t 所有字符的最短子串。如果不存在这样的子串，返回空字符串。
// t 中可能有重复字符，窗口中每个字符的数量必须不少于 t 中该字符的数量。
// 示例：
// 输入：s = "ADOBECODEBANC", t = "ABC"
// 输出："BANC"
// 约束：
// 1 <= s.length, t.length <= 10⁵
// s 和 t 由英文字母组成
// 答案唯一
// 解题思路:经典滑动窗口。用两个指针 left 和 right 维护一个窗口：right 右移扩大窗口，直到窗口包含了 t 的所有字符
// 此时 left 右移收缩窗口，尝试找更短的答案; 一旦窗口不再满足条件，回到步骤 1
// 关键是怎么快速判断"窗口是否包含 t 的所有字符"。用一个计数器 valid 记录已经满足的字符种类数，
// 等于 t 的字符种类数时就满足了。

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

string minWindow(string s, string t)   //source和target
{
    unordered_map<char, int> need; // t 里每个字符需要几个（固定不变）
    unordered_map<char, int> window; //当前窗口里每个字符有几个（一直在变）
    //每次判断某个字符够不够，就是拿 window[c] 和 need[c] 比较。
    //没有 need，你不知道要凑多少个才算够。没有 window，你不知道当前有几个。
    for (char c : t)
    {
        need[c]++;
    }
    int left = 0, right = 0; //窗口的左右边界
    int valid = 0; //已经满足的字符种类数
    int start = 0; //最小覆盖子串的起始索引
    int minLen = INT_MAX; //最小覆盖子串的长度

    while (right < s.size())
    {
        char c = s[right];
        right++; //右移窗口
        if (need.contains(c))
        {
            ++window[c];
            if (window[c] == need[c])
                valid ++;
        }
        while (valid == need.size()) //尝试收缩窗口
        {
            if (right-left < minLen)  //发现了更短子串的时候才更新start【start用于最终的返回值】
            {
                start = left;
                minLen = right-left;
            }
            char d = s[left];
            left++; //左移窗口
            if (need.contains(d))
            {
                if (window[d] == need[d])
                    valid --;
                window[d] --;
            }
        }
    }
    return minLen == INT_MAX ? "" : s.substr(start, minLen);
}
//画图就明白了
