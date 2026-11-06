
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

class Solution
{
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
        ans.push_back(node->val);  //根
        dfs(node->left, ans);
        dfs(node->right, ans);
    }

    //2.迭代
    vector<int> inorderTraversalB(TreeNode* root) {
        vector<int> ans;
        if (!root) return ans;
        stack<TreeNode*> stk;
        stk.push(root);
        while (!stk.empty())
        {
            TreeNode* node = stk.top();
            stk.pop();
            ans.push_back(node->val);
            if (node->right) stk.push(node->right);  // 先压右
            if (node->left) stk.push(node->left);  // 再压左，这样左先出栈
        }
        return ans;
    }
};