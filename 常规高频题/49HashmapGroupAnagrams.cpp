// 给定一个字符串数组 strs，把所有字母异位词（anagram）分到同一组。
// 字母异位词就是字母完全相同但顺序不同的单词。

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

vector<vector<string>> groupAnagrams(vector<string>& strings)
{
    unordered_map<string, vector<string>> groupAnagrams;
    for (string& s : strings)
    {
        string key = s;
        sort(key.begin(), key.end());
        groupAnagrams[key].push_back(s);
    }
    vector<vector<string>> result;
    for (auto& pair : groupAnagrams)
    {
        result.push_back(move(pair.second));
    }
    return result;
}