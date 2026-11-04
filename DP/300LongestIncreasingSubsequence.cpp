// 给定一个整数数组 nums，找出其中最长严格递增子序列的长度。
// 子序列不要求连续，但要保持相对顺序。
//
// 示例：
// 输入：nums = [10, 9, 2, 5, 3, 7, 101, 18]
// 输出：4
// 解释：最长递增子序列是 [2, 3, 7, 101] 或 [2, 3, 7, 18]，长度 4
// 解法一：动态规划 O(n²)
// dp[i] = 以 nums[i] 结尾的最长递增子序列长度。
// 对于每个 i，往前看所有 j < i，如果 nums[j] < nums[i]，说明 nums[i] 可以接在 nums[j] 后面。
#include <vector>
using namespace std;

int lengthOfLIS(vector<int>& nums) {
    int n = nums.size();
    if (n == 0) return 0;
    
    vector<int> dp(n, 1); // 初始化每个位置的最长递增子序列长度为 1
    int maxLength = 1;
    
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (nums[j] < nums[i]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
        maxLength = max(maxLength, dp[i]);
    }
    return maxLength;
}
// 解法二：贪心 + 二分 O(n log n)
// 维护一个数组 tails，tails[i] 表示长度为 i+1 的递增子序列的最小末尾元素。
// 遍历每个数，用二分查找决定它应该放在 tails 的哪个位置：
int lengthOfLIS(vector<int>& nums)
{
    vector<int> tails;
    for (int num: nums)
    {
        int left = 0, right = tails.size();
        while (left < right)
        {
            int mid = left + (right - left) / 2;
            if (tails[mid] < num)
                left = mid + 1;
            else
                right = mid;
        }
        if (left == tails.size())
            tails.push_back(num);
        else
            tails[left] = num;
    }
    return tails.size();
}