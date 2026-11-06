// 实现一个 Trie，支持：
// insert(word)：插入一个单词
// search(word)：查找单词是否存在（完整匹配）
// startsWith(prefix)：查找是否有以该前缀开头的单词

//方法：Trie 的每个节点有 26 个子指针（对应 a-z），加一个 isEnd 标记表示是否有单词在此结束。

class Trie {
    struct TrieNode {
        TrieNode* children[26] = {};  //进阶的话，可以换hashmap只存出现过的
        bool isEnd = false;
    };

    TrieNode* root;
public:
    Trie() {
        root = new TrieNode();
    }
    void insert(string word){
        TrieNode* cur = root;
        for(char c: word){
            int i = c - 'a';
            if(!cur->children[i]){              //这个字符不存在的话要创建
                cur->children[i] = new TrieNode();                    
            }
            cur = cur->children[i];
        }
        cur->isEnd = true;   //不要忘了这个标记
    }
    bool search(string word){
        TrieNode* cur = root;
        for(char c : word){
            int i = c - 'a';
            if(!cur->children[i]){
                return false;
            }
            cur = cur.children[i];
        }
        if(!cur->isEnd) return false;
        return true;
    }
    bool startsWith(string prefix){
        TrieNode* cur = root;
        for(char c : prefix){
            int i = c - 'a';
            if(!cur->children[i]){
                return false;
            }
            cur = cur.children[i];
        }
        //if(cur->isEnd) return false;  前缀自己也可以算个词的，不需要这个
        return true;
    }
private:

}；