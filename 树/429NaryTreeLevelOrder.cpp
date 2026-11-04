/*
 * LeetCode 429：N 叉树的层序遍历
 *
 * 给定一棵 N 叉树的根节点 root。每个节点包含一个整数值以及一个按顺序
 * 保存其所有子节点的列表 children。
 *
 * 请按层从上到下遍历这棵树，并返回一个二维整数数组：结果中的第 i 个
 * 子数组包含树中第 i 层的所有节点值，同一层内按从左到右的顺序排列。
 * 如果 root 为空，则返回空数组。
 */
#include <vector>
#include <queue>
using namespace std;

class Node {
public:
	int value;
	vector<Node*> children;
};

vector<vector<int>> levelOrder(Node* root) {
	vector<vector<int>> result;
	if (!root) return result;

	queue<Node*> q;
	q.push(root);
	while (!q.empty()) {
		vector<int> level;
		int size = q.size(); // BFS，总是需要记层数
		for (int i = 0; i < size; ++i)
		{
			Node* cur = q.front();
			q.pop();
			level.push_back(cur->value);
			for (Node* child : cur->children) {
				q.push(child);
			}
		}
		result.push_back(level);
	}
	return result;
}