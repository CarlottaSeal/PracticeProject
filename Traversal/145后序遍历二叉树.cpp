// 题目：给定一棵二叉树的根节点，返回它的后序遍历结果（左 → 右 → 根）。
// 例子：
//     1
//       \
//       2
//      /
//     3
// 输出：[3, 2, 1]
#include <vector>
#include <queue>
#include <stack>
using namespace std;
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    //1. dfs
    vector<int> postorderTraversalD(TreeNode* root) {
        vector<int> result;
        dfs(root, result);
        return result;
    }
    void dfs(TreeNode* node, vector<int>& result) {
        if (!node) return;
        
        dfs(node->left, result);
        dfs(node->right, result);
        result.push_back(node->val);
    }

    //2.迭代
    //后序是 左→右→根，而前序是 根→左→右。如果把前序改成 根→右→左，再反转结果，就得到 左→右→根：
    vector<int> postorderTraversalB(TreeNode* root) {
        vector<int> result;
        if (!root) return result;

        stack<TreeNode*> s;
        s.push(root);
        while (!s.empty())
        {
            TreeNode* node = s.top();
            s.pop();
            result.push_back(node->val);
            if (node->left) s.push(node->left); //先压左
            if (node->right) s.push(node->right);  // 再压右，这样右先出栈
        }
        reverse(result.begin(), result.end());
        return result;
    }
};