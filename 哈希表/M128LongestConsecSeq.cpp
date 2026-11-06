//给定一个未排序的整数数组 nums，找出其中最长的连续元素序列的长度。
//要求算法的时间复杂度为 O(n)。
//
//示例：
//输入：nums = [100, 4, 200, 1, 3, 2]
//输出：4
//解释：最长连续序列是 [1, 2, 3, 4]，长度为 4

//先把所有数丢进一个 unordered_set。然后遍历每个数，只有当它是某段连续序列的起点时，才开始往后数。
//怎么判断是不是起点？如果 num - 1 不在 set 里，说明它前面没有数了，它就是起点。
//这样每段连续序列只会被数一次，总时间 O(n)。

#include <algorithm>
#include <unordered_set>
#include <vector>
using namespace std;

int longestConsecutive(vector<int>& nums) {
    unordered_set<int> s(nums.begin(), nums.end());
    int longestStreak = 0;
    for (int num : s) {
        if (!s.contains(num - 1)) { // 只从序列的起点开始计数
                int currentNum = num;
                int currentStreak = 1;
                while (s.contains(currentNum + 1)) {
                    currentNum++;
                    currentStreak++;
                }
                longestStreak = max(longestStreak, currentStreak);
        }
    }
    return longestStreak;
}