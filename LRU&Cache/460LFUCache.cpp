// 设计并实现一个**最不经常使用（LFU）**缓存。实现 LFUCache 类：
// LFUCache(int capacity)：初始化缓存，容量为 capacity
// int get(int key)：如果 key 存在，返回其值，否则返回 -1
// void put(int key, int value)：如果 key 存在，更新其值；不存在则插入。
// 当缓存满了需要淘汰时，淘汰使用频次最低的 key；如果频次相同，淘汰最久未使用的那个（LRU）
// get 和 put 的调用都算一次"使用"，频次 +1。要求 get 和 put 均为 O(1) 时间复杂度。

// 约束：
// 1 <= capacity <= 10⁴
// 0 <= key <= 10⁵
// 0 <= value <= 10⁹
// 最多调用 2 × 10⁵ 次 get 和 put
//同频次的 key 共享一条链表。用iterator是因为list虽然剔除o1但是找key不是，所以用iterator列表维护
#include <list>
#include <unordered_map>
using namespace std;
class LFUCache
{
    struct Node {
        int key, val, freq;
        Node(int k, int v, int freq = 1) : key(k), val(v), freq(freq) {}
    };
    int capacity;
    int minFreq;
    unordered_map<int, list<Node>::iterator> keyToIt;
    unordered_map<int, list<Node>> freqToList;

    void touch(list<Node>::iterator it) //碰一下，更新频次和位置
    {
        int freq = it->freq;
        int key = it->key;
        int val = it->val;

        freqToList[freq].erase(it); //旧节点删了 44行需要重新链接
        if (freqToList[freq].empty())
        {
            freqToList.erase(freq);
            if (minFreq == freq)
                minFreq++;
        }

        freq ++;
        freqToList[freq].push_front(Node(key,val, freq));//push了新节点，44行需要指向它
        keyToIt[key] = freqToList[freq].begin(); 
    }

public:
    LFUCache(int capacity) : capacity(capacity), minFreq(0) {}

    int get(int key)
    {
        if (!keyToIt.contains(key)) return -1;

        auto it = keyToIt[key];
        int val = it->val;
        touch(it);
        return val;
    }
    void put(int key, int val)
    {
        if (capacity == 0) return;
        if (keyToIt.contains(key))
        {
            auto it = keyToIt[key];
            it->val = val;
            touch(it);
        }
        else
        {
            if ((int)keyToIt.size() == capacity)
            {
                auto it = freqToList[minFreq].back(); // 淘汰 minFreq 链表的尾部（最久未使用）
                keyToIt.erase(it.key);
                freqToList[minFreq].pop_back();
                if (freqToList[minFreq].empty())
                    freqToList.erase(minFreq);
            }
            minFreq = 1;
            freqToList[1].push_front(Node(key,val, 1));
            keyToIt[key] = freqToList[minFreq].begin();
        }
    }
};
