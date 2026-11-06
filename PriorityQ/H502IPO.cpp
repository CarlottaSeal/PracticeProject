//你有初始资金 w，最多可以完成 k 个项目。每个项目有一个最低启动资金 capital[i] 和完成后的纯利润 profits[i]。
// 完成一个项目后，利润会加到你的总资金上。求最终能获得的最大资金。

//排序+贪心：总是选利润最大的即可->greater queue
//但captial和profits的index一一对应，排序前要注意；可以打包成pair重写
#include <algorithm>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

class Solution {
public:
    int findMaximizedCapital(int k, int w, vector<int>& profits, vector<int>& capital) {
        vector<pair<int, int>> projects(profits.size());
        for(int i = 0; i < profits.size(); i++){
            projects[i] = {capital[i], profits[i]};
        }
        sort(projects.begin(), projects.end());

        int n = projects.size();
        priority_queue<int> maxHeap;   //堆里放利润即可
        int i = 0;
        while(k --){
            while(i < n && projects[i].first <= w){
                maxHeap.push(projects[i].second);
                i++;
            }
            if(maxHeap.empty()) break;  //堆问题在top和pop前一定要检查空
            
            w += maxHeap.top();
            maxHeap.pop();
        }

        return w;
    }
};