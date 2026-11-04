// 给定一个无重复元素的整数数组 candidates 和一个目标值 target，找出 candidates 中所有可以使数字之和等于 target 的组合。
// candidates 中的同一个数字可以被无限次重复选取。
// 结果不能包含重复的组合。
// 示例：
// 输入：candidates = [2, 3, 6, 7], target = 7
// 输出：[[2, 2, 3], [7]]
// 解释：2+2+3=7，7=7
//
// 输入：candidates = [2, 3, 5], target = 8
// 输出：[[2, 2, 2, 2], [2, 3, 3], [3, 5]]
#include <algorithm>
#include <unordered_map>
#include <vector>
using namespace std;

vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
    vector<vector<int>> res;
    vector<int> path;
    sort(candidates.begin(), candidates.end());
    backtrack(candidates, target, 0, path, res);
    return res;
}
void backtrack(vector<int>& candidates, int target, int start,
                vector<int>& path, vector<vector<int>>& res)
{
    if (target == 0)
    {
        res.push_back(path);
        return;
    }
    for (int i = start; i < candidates.size(); ++i)
    {
        if (candidates[i] > target) break; // 排过序了，后面都更大，直接剪掉
        
        path.push_back(candidates[i]);
        backtrack(candidates, target - candidates[i], i, path, res); // 允许重复使用当前元素（所以不用++i，start还是i）
        path.pop_back();
    }
}