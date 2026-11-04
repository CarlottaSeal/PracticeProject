// 给定一个不含重复元素的整数数组 nums，返回其所有可能的子集（幂集）。
// 结果中不能包含重复的子集，可以按任意顺序返回。
// 示例：
// 输入：nums = [1, 2, 3]
// 输出：[[], [1], [2], [3], [1,2], [1,3], [2,3], [1,2,3]]
// 约束：
// 1 <= nums.length <= 10
// -10 <= nums[i] <= 10
// 所有元素互不相同
//对于每个元素，只有两种选择：选或不选。用回溯逐个做决定就行。
#include <vector>
using namespace std;

vector<vector<int>> subsets(vector<int>& nums) {
    vector<vector<int>> subsets;
    vector<int> path;
    backtrack(nums, 0, path, subsets);
    return subsets;
}

void backtrack(vector<int>& nums, int i, vector<int>& path,
              vector<vector<int>>& subsets) {
    if (i == nums.size())
    {
        subsets.push_back(path);
        return;
    }
    //不选nums[i]
    backtrack(nums, i + 1, path, subsets);

    //选nums[i]
    path.push_back(nums[i]);
    backtrack(nums, i + 1, path, subsets);
    path.pop_back();
}

//另一种写法：枚举下一个选谁
void backtrack(vector<int>& nums, int i, vector<int>& path,
              vector<vector<int>>& subsets)
{
    subsets.push_back(path);   //每次都把当前path加入结果，每个节点都是一个合法子集
    for (int j = i; j < nums.size(); ++j)
    {
        path.push_back(nums[j]);               // 修正：原来写成 nums[i]
        backtrack(nums, j + 1, path, subsets); // 修正：原来写成 i + 1
        path.pop_back();
    }
}