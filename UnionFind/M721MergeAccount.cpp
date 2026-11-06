// 给定一组账户，每个账户是 [name, email1, email2, ...]。
// 如果两个账户有相同的邮箱，说明它们属于同一个人，需要合并。返回合并后的账户，每个账户的邮箱按字典序排列。
// 输入：[["John","a@","b@"], ["John","b@","c@"], ["Mary","d@"]]
// 输出：[["John","a@","b@","c@"], ["Mary","d@"]]
// 前两个账户共享 "b@"，所以合并

class Solution {
    vector<int> parent;
    vector<int> rank_;

    int find(int x) {           //这是在找root
        if(parent[x] != x){    //parent是自己，说明自己就是root，不用再找了
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if(ra == rb) return;
        if(rank_[ra] < rank_[rb]){
            swap(ra, rb);
        }
        parent[rb] = ra;
        if(rank_[ra] == rank_[rb]){
            rank_[ra] ++;               //ra要加一层，因为我们把rb接到ra下了
        }                               //rank 不是"每条路的精确深度"，它是整棵树最大可能高
    }
    // rank 本质上是个保守估计——它保证 find() 最多跳 rank 次，但实际上很多路径比 rank 短。
    // 路径压缩会让实际跳数更少，但 rank 不会因此更新（所以叫上限，不叫精确高度）。度的上限。

public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        unordered_map<string, int> emailToId;   // 邮箱 → 编号
        unordered_map<string, string> emailToName; // 邮箱 → 人名
        //并查集的parent里都是int，不是字符串 要对应起邮箱->编号
        //最后需要邮箱->人名
        // 1. 给每个邮箱分配编号，记录对应人名
        int id = 0;
        for(auto& acc : accounts){
            for(int i = 1; i < acc.size(); i++){
                if(!emailToId.contains(acc[i])){
                    emailToId[acc[i]] = id++;
                }
                emailToName[acc[i]] = acc[0];
            }
        }
        // 2. 初始化并查集
        parent.resize(id);   //一共这些款邮箱
        rank_.resize(id, 0);   

        //3. 同一账户内邮箱合并
        for(auto& acc : accounts){
            int first = emailToId[acc[1]]; //key是email，找到第一个邮箱的id
            for(int i = 2; i < acc.size(); i++){   //从第二个邮箱继续
                unite(first, emailToId[acc[i]]);
            }
        }

        //4.按根节点分组
        unordered_map<int, set<string>> groups;
        for(auto& [email, eid] : emailToId){
            groups[find(eid)].insert(email);  //set可以自动排序，题要求了字典序
        }



        //5.组装结果
        vector<vecotor<string>> result;
        for(auto& [root, emails] : groups){
            vector<string> merged;
            string anyEmail = *emails.begin(); //第一个邮箱，注意写法
            merged.push_back(emailToName[anyEmail]);
            for(auto& e : emails){
                merged.push_back(e);
            }
            result.push_back(merged);
        }

        return result;
    }
};