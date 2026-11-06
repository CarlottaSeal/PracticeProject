// 设计一个链表，支持以下操作：
// get(index)：获取第 index 个节点的值
// addAtHead(val)：在头部插入
// addAtTail(val)：在尾部插入
// addAtIndex(index, val)：在第 index 个节点前插入
// deleteAtIndex(index)：删除第 index 个节点

// 链表总是要dummy的head和尾->避免边界判断

class MyLinkedList {
    struct Node {
        int val;
        Node* prev;
        Node* next;
        Node(int v) : val(v), prev(nullptr), next(nullptr) {}
    };

    Node* head; // 哨兵头
    Node* tail; // 哨兵尾
    int size;

public:
    MyLinkedList(): size(0){
        Node* head = new Node(0);
        Node* tail = new Node(0);
        head->next = tail;
        tail->prev = head;
    } 

    void addAtHead(int val){
        addAfter(head, val);
    }

    void addAtTail(int val){
        addAfter(tail->prev, val);
    }

    void addAtIndex(int index, int val){  // 插入到第 index 个节点之前 = 插入到第 index-1 个节点之后
        if(index < 0 || index > size){
            return;
        }
        if(index == size){
            addAtTail(val);
            return;
        }

        Node* cur = getNode(index);
        addAfter(cur->prev, val);
        return;
    }

    void deleteAtIndex(int index){
        if(index < 0 || index > size){
            return;
        }
        Node* cur = getNode(index);
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;
        delete cur;
        cur = nullptr;
        size --;
    }

private:
    void addAfter(Node* node, int val){
        Node* toAdd = new Node(val);
        toAdd->next = node->next;
        toAdd->prev = node;
        node->next->prev = toAdd;
        node->next = toAdd;
        size ++;
    }

    Node* getNode(int index){
        Node* cur;
        if(index < size/2){
            cur = head->next;
            for(int i = 0; i < index; i++){
                cur = cur->next;
            }
        }
        else{
            cur = tail->prev;
            for(int i = size-1; i > index; i--){
                cur = cur->prev;
            }
        }
        return cur;
    }
};