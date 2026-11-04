// 给定一个升序排列的整数数组 nums，数组中的值互不相同。
// 在某个未知下标处进行了旋转（例如 [0,1,2,4,5,6,7] 旋转后变成 [4,5,6,7,0,1,2]）。
// 给定一个目标值 target，如果存在返回其下标，否则返回 -1。
// 要求时间复杂度 O(log n)。
//
// 示例：
// 输入：nums = [4,5,6,7,0,1,2], target = 0
// 输出：4
// 输入：nums = [4,5,6,7,0,1,2], target = 3
// 输出：-1
// 约束：
// 1 <= nums.length <= 5000
// -10⁴ <= nums[i] <= 10⁴
// 所有值互不相同
// nums 一定是某个升序数组的旋转
// 解题思路
// 要求 O(log n)，一定是二分。关键是旋转后数组不完全有序，但有一个性质：二分后，左右两半一定有一半是有序的。

#include <algorithm>
#include <unordered_map>
#include <vector>
using namespace std;

int search(vector<int>& nums, int target)
{
    int left = 0;
    int right = nums.size() - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target)
        {
            return mid;
        }
        // Check if the left half is sorted
        if (nums[left] <= nums[mid])
        {
            if (nums[left] <= target && target < nums[mid])
            {
                right = mid - 1;
            }
            else{
                left = mid + 1;
            }
        }
        else
        {
            if (nums[mid] < target && target <= nums[right]) // 修正：原来是 <= 和 <，target == nums[right] 会漏掉
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }
    }
        return -1;
}