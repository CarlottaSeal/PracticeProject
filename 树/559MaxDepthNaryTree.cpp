/*
 * LeetCode 559：N 叉树的最大深度
 *
 * 给定一棵 N 叉树的根节点 root。每个节点包含一个整数值以及零个或多个
 * 子节点。
 *
 * 节点的深度定义为从根节点到该节点的路径上所包含的节点数；因此根节点
 * 的深度为 1。请返回整棵树中所有节点深度的最大值。若 root 为空，则树
 * 的最大深度为 0。
 */
#include <vector>
#include <queue>
using namespace std;

class Node {
public:
	int value;
	vector<Node*> children;
};
//DFS
int maxDepth(Node* root) {
	int depth = 0;
	if (!root) return 0;
	for (Node* child : root->children)
	{
		depth = max(depth, maxDepth(child));
	}
	return depth + 1;
}
//BFS
int maxDepth(Node* root) {
	int depth = 0;
	if (!root) return 0;
	queue<Node*> q;
	q.push(root);   
	while (!q.empty()) {
		int size = q.size();   //必须有这个size，因为q的size动态变化，我们不要增长的q，只要push子节点前的固定大小（这一层的数量）
		depth++;
		for (int i = 0; i < size; ++i) {
			Node* cur = q.front();
			q.pop();
			for (Node* child : cur->children) {
				q.push(child);
			}
		}
	}
	return depth; // 修正：原来漏了 return
}