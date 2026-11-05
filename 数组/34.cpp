//给定一个升序排列的整数数组 nums 和一个目标值 target，找出 target 在数组中的起始位置和结束位置。要求时间复杂度 O(log n)。找不到返回 [-1, -1]。
//二分查找两次，左右侧各做二分查找
//1. 找左边界（第一个 >= target 的位置）
//1. 找右边界（最后一个 <= target 的位置，等价于第一个 > target 的位置减 1）
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int left = lowerBound(nums, target);
        if(left == nums.size() || nums[left] != target){  //不存在
            return {-1, -1};
        }

        int right = lowerBound(nums, target + 1) - 1; //找第一个>=target+1的位置，-1就是要找的结束位置
        return {left, right};
    }

private:
    // lowerBound准确定义：第一个 >= target 的位置。它不关心 target 存不存在。
    int lowerBound(vector<int>& nums, int target) {
        int lo = 0;
        int hi = nums.size();  //hi=size 左闭右开区间
        while(lo < hi){
            int mid = lo + (hi - lo) / 2;
            if(nums[mid] < target){
                lo = mid + 1;
            }
            else{
                hi = mid;
            }
        }
        return lo;
    }
};


//左闭右开不容易写错：右边收缩时直接 hi = mid 而不是 mid - 1，少一个 ±1
// // 左闭右开 [lo, hi)
// int lo = 0, hi = nums.size();
// while (lo < hi) {
//     int mid = lo + (hi - lo) / 2;
//     if (nums[mid] < target) lo = mid + 1;
//     else hi = mid;
// }

// // 左闭右闭 [lo, hi]
// int lo = 0, hi = nums.size() - 1;
// while (lo <= hi) {
//     int mid = lo + (hi - lo) / 2;
//     if (nums[mid] < target) lo = mid + 1;
//     else hi = mid - 1;
// }
// // 最终 lo 就是答案