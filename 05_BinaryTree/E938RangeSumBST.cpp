//给定一棵二叉搜索树（BST）的根节点 root，和两个整数 low、high。
//返回所有值在 [low, high] 范围内的节点值之和。
// 暴力做法是遍历所有节点，把在范围内的加起来。但这是 BST(左子树所有节点 < 根 < 右子树所有节点)，可以利用有序性剪枝：
// 当前节点值 < low → 左子树全部更小，不用看，只去右子树
// 当前节点值 > high → 右子树全部更大，不用看，只去左子树
// 当前节点值在范围内 → 加上它，左右都要看
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
    int rangeSumBST(TreeNode* root, int low, int high)
    {
        if (!root) return 0;
        
        if (root->val < low)
            return rangeSumBST(root->right, low, high);
        if (root->val > high)
            return rangeSumBST(root->left, low, high);

        return root->val
                + rangeSumBST(root->left, low, high)
                + rangeSumBST(root->right, low, high);
    }
};

