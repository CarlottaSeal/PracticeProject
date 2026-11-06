// 设计一个数据结构，支持：
// addWord(word)：添加一个单词
// search(word)：搜索单词，. 可以匹配任意一个字符
// 示例：
// addWord("bad")
// addWord("dad")
// search("b.d") → true   （匹配 "bad"）
// search("b..") → true   （匹配 "bad"）
// search("..d") → true   （匹配 "bad" 或 "dad"）
// search("xyz") → false

// Trie+BFS:每个位置都是一层，找当前层里有没有符合要求的（.的话直接跳到下一层）
// 但是： Trie+DFS更好：因为找到可以立刻返回；BFS会加入整层所有节点到queue中处理

#include <string>
using namespace std;

class WordDictionary {
    struct TrieNode {
        TrieNode* children[26] = {};
        bool isEnd = false;
    };

    TrieNode* root;

public:
    WordDictionary() {
        root = new TrieNode();
    }
    
    void addWord(string word){   //和208一模一样
        TrieNode* cur = root;
        for(char c: word){
            int i = c - 'a';
            if(!cur->children[i]){              
                cur->children[i] = new TrieNode();                    
            }
            cur = cur->children[i];
        }
        cur->isEnd = true;   
    }

    bool search(string word){
        return dfs(root, word, 0);
    }

private:
    bool dfs(TrieNode* start, string word, int index){
        if(!start) return false;
        if(index == word.size()) return start->isEnd;

        if(word[index] == '.'){
            for(int i = 0; i < 26; i++){
                if(dfs(start->children[i], word, index +1 )){
                    return true;
                }
                //这条路没走通的话换下一个 ∴不能在if里直接return dfs(...)
            }
            return false;
        }
        else{
            int i = word[index] - 'a';
            // if(!start->children[i]){    //递归第一行兜底了，不需要这个
            //     return false;
            // }
            return dfs(start->children[i], word, index+1); //index如果要写++必须是++index，否则就是先传入再赋值了
        }
    }
};