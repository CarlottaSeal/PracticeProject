// 给定一个不含重复数字的整数数组 nums，返回它的所有全排列。可以按任意顺序返回。
// 示例：
// 输入：nums = [1, 2, 3]
// 输出：[[1,2,3], [1,3,2], [2,1,3], [2,3,1], [3,1,2], [3,2,1]]
//
// 输入：nums = [1]
// 输出：[[1]]
//
// 约束：
// 1 <= nums.length <= 6
// -10 <= nums[i] <= 10
// 所有数字互不相同

// 经典回溯。每一层选一个还没用过的数放到当前位置，选完所有位置就得到一个排列。
// 用一个 used 数组标记哪些数已经被选过，避免重复选。
#include <algorithm>
#include <unordered_map>
#include <vector>
using namespace std;

vector<vector<int>> permute(vector<int>& nums)
{
    vector<vector<int>> result;
    vector<int> path;
    vector<bool> used(nums.size(), false);
    backtrack(nums, used, path, result);
    return result;
}

void backtrack(vector<int>& nums, vector<bool>& used,
                  vector<int>& path, vector<vector<int>>& res)
{
    if (path.size() == nums.size())
    {
        res.push_back(path);
        return;    //回溯结束！
    } 
    for (int i =0; i < nums.size(); ++i)
    {
        if (used[i]) continue;
        
        used[i] = true;
        path.push_back(nums[i]);
        backtrack(nums, used, path, res);

        //撤销选择
        path.pop_back();
        used[i] = false;
    }
}