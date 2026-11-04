// 设计一个满足 LRU（最近最少使用）缓存约束的数据结构。实现 LRUCache 类：
//
// LRUCache(int capacity)：初始化容量为 capacity 的缓存
// int get(int key)：key 存在则返回对应 value，否则返回 -1
// void put(int key, int value)：key 存在则更新 value；不存在则插入。如果插入后超出容量，淘汰最久未使用的那个 key
//
// get 和 put 都必须是 O(1) 时间。

#include <unordered_map>
using namespace std;
class LRUCache
{
    struct Node {
        int key, val;
        Node* prev;
        Node* next;
        Node(int k = 0, int v = 0) : key(k), val(v), prev(nullptr), next(nullptr) {}
    };
    int capacity;
    unordered_map<int, Node*> cache;
    Node* head;
    Node* tail;

    void remove(Node* node)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    void addToFront(Node* node)
    {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }
public:
    LRUCache(int capacity) : capacity(capacity)
    {
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
    }
    int get(int key)
    {
        if (cache.find(key) == cache.end())
            return -1;
        Node* node = cache[key];
        remove(node);
        addToFront(node);
        return node->val;
    }
    void put(int key, int value)
    {
        if (cache.find(key) != cache.end())
        {
            Node* node = cache[key];
            remove(node);
            node->val = value;
            addToFront(node);
        }
        else
        {
            Node* node = new Node(key, value);
            cache[key] = node;
            addToFront(node);
            if ((int)cache.size() > capacity)
            {
                Node* prev = tail->prev;
                remove(prev);
                cache.erase(prev->key);
                delete prev;
            }
        }
    }
};
