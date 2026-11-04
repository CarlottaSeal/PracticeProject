/*
 * LeetCode 428：序列化和反序列化 N 叉树
 *
 * 序列化是把内存中的数据结构转换为可存储或传输的字符序列；反序列化
 * 则根据该字符序列重建原数据结构。给定一棵 N 叉树，每个节点包含一个
 * 整数值以及零个或多个有序子节点。
 *
 * 请实现 Codec 类中的两个方法：serialize(root) 把整棵 N 叉树编码为
 * 字符串，deserialize(data) 根据字符串恢复出结构和值都相同的 N 叉树。
 * 编码格式可自行设计，但必须保证任意合法 N 叉树都能被唯一、正确地还原；
 * 对空树也需要正确处理。
 */
#include <vector>
#include <string>
using namespace std;
class Codec {
	class Node {
	public:
		int value;
		vector<Node*> children;

		Node() {}
		Node(int _val) : value(_val) {}
		Node(int _val, vector<Node*> _children) : value(_val), children(_children) {}
	};
public:
	string serialize(Node* root) {
		if (!root) return "";
		string result = to_string(root->value) + " " + to_string(root->children.size()) + " ";
		for (auto child : root->children) {
			result += serialize(child);
		}
		return result;
	}

	Node* deserialize(string data) {
		if (data.empty()) return nullptr;
		vector<string> tokens;
		string token;
		for (char c : data) {
			if (c == ' ') {
				tokens.push_back(token);
				token.clear();
			} else {
				token += c;
			}
		}
		if (!token.empty()) tokens.push_back(token);
		int i = 0;
		return build(tokens, i);
	}

private:
	Node* build(vector<string>& tokens, int& i) //必须是引用，因为要在递归中更新 i 的值
	{
		int val = stoi(tokens[i]);
		i++;
		int size = stoi(tokens[i]);
		i++;
		Node* root = new Node(val);
		for (int j = 0; j < size; ++j) {
			Node* child = build(tokens, i);
			root->children.push_back(child);
		}
		return root;
	}
};