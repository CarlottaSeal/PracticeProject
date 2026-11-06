
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
    vector<int> inorderTraversalD(TreeNode* root) {
        vector<int> ans;
        dfs(root, ans);
        return ans;
    }

private:
    void dfs(TreeNode* node, vector<int>& ans) {
        if (!node) return;
        dfs(node->left, ans);
        ans.push_back(node->val);  //根
        dfs(node->right, ans);
    }

    //2.迭代
    vector<int> inorderTraversalB(TreeNode* root) {
        vector<int> ans;
        stack<TreeNode*> stk;
        TreeNode* cur = root;
        while (cur || !stk.empty())
        {
            // 一路向左，全部入栈
            while (cur)
            {
                stk.push(cur);
                cur = cur->left;
            }
            // 左子树为空，出栈访问根节点
            cur = stk.top();
            stk.pop();
            ans.push_back(cur->val);
            // 转向右子树
            cur = cur->right;
        }
        return ans;
    }
};