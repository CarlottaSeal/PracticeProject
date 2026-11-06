// 给定一个单链表的头节点 head，将链表反转，返回反转后的头节点。
// 示例：
// 输入：1 → 2 → 3 → 4 → 5
// 输出：5 → 4 → 3 → 2 → 1
// 输入：1 → 2
// 输出：2 → 1
// 输入：[]
// 输出：[]
//
// 遍历链表，把每个节点的 next 指针掉头，指向前一个节点。需要三个指针：
// prev：当前节点反转后应该指向谁（初始为 nullptr）
// curr：当前正在处理的节点
// next：提前保存下一个节点，防止改了 next 指针之后找不到后面的节点

// LeetCode 给的定义
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

ListNode* reverseList(ListNode* head)
{
    ListNode* prev = nullptr;
    ListNode* curr = head;
    while (curr)
    {
        ListNode* next = curr->next;//先存下一个
        curr->next = prev;         //掉头
        prev = curr;                //prev前进
        curr = next;                //next前进
    }
    return prev;
}
