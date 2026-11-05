// 给定 beginWord、endWord 和一个字典 wordList，每次只能改一个字母，且中间词必须在字典中，求最短转换序列的单词数。不存在返回 0。
//（每次只能改一个字母，而且改完之后的词必须是字典里的合法单词，问你最少改几步能从起始词变到目标词。）
// 思路：经典 BFS 最短路。每个单词是一个节点，只差一个字母的两个单词之间连一条边，求从起点到终点的最短路。所以用 BFS
// 关键优化是用通配符模式（h*t 匹配 hot/hit）建邻接关系，避免逐对比较。

class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> wordSet(wordList.begin(), wordList.end());
        if(!wordSet.contains(endWord)) return 0;

        queue<string> queue<int> q;
        q.push(beginWord);
        wordSet.erase(beginWord);
        int steps = 1;

        while (!q.empty()){
            int size = q.size();
            while (size --){
                string cur = q.front();
                q.pop();
                for(int i = 0; i < (int)cur.size(); i++){
                    char orig = cur[i];
                    for (char c = 'a'; c <= 'z'; c++){   //找‘改一个就变成当前’的词，push为一层做bfs
                        if(c == orig) continue;
                        cur[i] = c;
                        if(cur == endWord)
                            return steps + 1;
                        if(wordSet.contains(cur)){
                            wordSet.erase(cur);
                            q.push(cur);
                        }
                        cur[i] = orig;   //改的那一位复原，继续找下一个满足这个的词
                    }
                }
            }
            steps++;
        }

        return 0;
    }
};