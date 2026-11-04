/*
 * LeetCode 23：合并 K 个升序链表
 *
 * 给定一个链表数组 lists，其中每个链表都已经按节点值从小到大排列。
 * 数组中的链表可能为空，数组本身也可能不包含任何链表。
 *
 * 请把所有链表中的节点合并成一个新的升序链表，并返回合并后链表的
 * 头节点。如果所有输入链表都为空，则返回空指针。
  */
#include <vector>
#include <queue>
#include <stack>
using namespace std;

struct ListNode {
	int val;
	ListNode* next;
	ListNode() : val(0), next(nullptr) {}
	ListNode(int x) : val(x), next(nullptr) {}
	ListNode(int x, ListNode* next) : val(x), next(next) {}
};

//1.归并
ListNode* mergeKLists(vector<ListNode*>& lists) {
	if (lists.empty()) return nullptr;
	return mergeSort(lists, 0, lists.size() - 1);
}

ListNode* mergeSort(vector<ListNode*>& lists, int left, int right) {
	if (left == right) return lists[left];
	int mid = left + (right - left) / 2;
	ListNode* l1 = mergeSort(lists, left, mid);
	ListNode* l2 = mergeSort(lists, mid + 1, right);
	return mergeTwoLists(l1, l2);
}

ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
	ListNode dummy(0);
	ListNode* tail = &dummy;
	while (l1 && l2) {
		if (l1->val < l2->val) {
			tail->next = l1;
			l1 = l1->next;
		}
		else {
			tail->next = l2;
			l2 = l2->next;
		}
		tail = tail->next;
	}
	tail->next = l1 ? l1 : l2;
	return dummy.next;
}
//2.最小堆  
struct Compare {
	bool operator()(ListNode* a, ListNode* b) {
		return a->val > b->val; //greater,∴小顶堆
	}
};
ListNode* mergeKLists(vector<ListNode*>& lists) {
	priority_queue<ListNode*, vector<ListNode*>, Compare> pq;

	for (ListNode* list : lists) {
		if (list) pq.push(list);
	}
	ListNode dummy(0);
	ListNode* tail = &dummy;
	while (!pq.empty()) {
		ListNode* node = pq.top();
		pq.pop();
		tail->next = node;
		tail = tail->next;
		if (node->next) pq.push(node->next);
	}
	return dummy.next;
}

