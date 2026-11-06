// 给定一个字符串数组 strs，将字母异位词分组。字母异位词是指字母相同但排列不同的字符串（如 "eat" 和 "tea"）。可以按任意顺序返回结果。
//
// 示例：
// 输入：strs = ["eat","tea","tan","ate","nat","bat"]
// 输出：[["eat","tea","ate"],["tan","nat"],["bat"]]
//
// 约束：
// 1 <= strs.length <= 10⁴
// 0 <= strs[i].length <= 100
// strs[i] 仅包含小写英文字母
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

vector<vector<string>> groupAnagrams(vector<string>& strs)
{
    unordered_map<string, vector<string>> anagrams;
    for (int i = 0; i < strs.size(); ++i)
    {
        string sortedStr = strs[i];
        sort(sortedStr.begin(), sortedStr.end());
        anagrams[sortedStr].push_back(strs[i]);
    }
    vector<vector<string>> result;
    for (auto& pair : anagrams)
    {
        result.push_back(move(pair.second));
    }
    return result;
}