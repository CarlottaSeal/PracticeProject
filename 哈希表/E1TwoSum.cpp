// 给定一个整数数组 nums 和一个整数目标值 target，在数组中找出和为目标值的两个数，返回它们的下标。
// 假设每组输入只有一个答案，且同一个元素不能重复使用。可以按任意顺序返回答案。
#include <unordered_map>
#include <vector>
using namespace std;

vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> numToIndex;  //nums that seen
    for (int i = 0; i < nums.size(); ++i) {
        int complement = target - nums[i];
        if (numToIndex.contains(complement))
        {
            return {numToIndex[complement], i};
        }
        numToIndex[nums[i]] = i;
    }
    return {};
}