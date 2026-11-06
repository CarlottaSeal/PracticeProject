//给定一棵二叉树，求任意两个节点之间最长路径的边数（即直径）。路径不一定经过根节点。

//对于任意节点，经过它的最长路径 = 左子树深度 + 右子树深度。∴用 DFS 后序遍历，每个节点返回自己的深度，同时在遍历过程中用一个全局变量记录 左深度 + 右深度 的最大值。
#include <algorithm>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    int ans = 0;
    //  空节点定义深度-1，叶子深度0 (这样叶子的深度=max(-1,-1) + 1 =>为0)
    int depth(TreeNode* node) {
        if (!node) return -1;
        int left = depth(node->left);
        int right = depth(node->right);

        ans = max(ans, left + right + 2); // 经过当前节点的路径长度
        return max(left, right) + 1;       // 返回当前节点的深度
    }

    int diameterOfBinaryTree(TreeNode* root) {
        depth(root);
        return ans;
    }
};