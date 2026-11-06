// 给定一个整数数组 nums，找出所有和为 0 的三元组 [nums[i], nums[j], nums[k]]，满足 i != j != k。
//
// 答案中不能包含重复的三元组。
#include <algorithm>
#include <unordered_map>
#include <vector>
using namespace std;

vector<vector<int>> threeSum(vector<int>& nums)
{
    //先排序；固定一个数，双指针找剩下两个数
    sort(nums.begin(), nums.end());
    vector<vector<int>> result;
    int n = nums.size();
    for (int i = 0; i < n - 2; i++)
    {
        if (nums[i] > 0) break;
        if (i > 0 && nums[i] == nums[i - 1]) continue; // 去重
        int left = i + 1;
        int right = n - 1;
        while (left < right)
        {
            int sum = nums[i] + nums[left] + nums[right];
            if (sum == 0)
            {
                result.push_back({nums[i], nums[left], nums[right]});
                while (left < right && nums[left] == nums[left + 1]) left++;
                while (left < right && nums[right] == nums[right - 1]) right--;
                left++;
                right--;
            }
            else if (sum < 0)
            {
                left++;
            }
            else
            {
                right--;
            }
        }
    }
    return result;
}