//给定一棵二叉树，找到路径和最大的路径。路径可以从任意节点出发到任意节点（不一定经过根），但路径中每个节点最多出现一次。

// 跟 543 题（直径）非常像.只是把"深度"换成了"最大路径和"。
//对于每个节点，经过它的路径和 = node->val + 左链最大贡献 + 右链最大贡献。
//"贡献"指的是从子节点出发向下走一条链能拿到的最大和。如果贡献为负，我们不取（贡献设为 0），因为不走比走更好。

class Solution {
public:
    int ans = INT_MIN;

    int maxGain(TreeNode* node) {
        //if(!node || node->value < 0) return 0; //不对，因为即使根是0，也可以加上很大的child让结果变大
        if(!node) return 0; 

        int leftMax = max(0, maxGain(node->left));
        int rightMax = max(0, maxGain(node->right));    //贡献为负的话取0

        ans = max(ans, node->val + leftMax + rightMax);

        // 返回当前节点能向上提供的最大贡献（只能选一条链）
        return node->val + max(left, right);
    }

    int maxPathSum(TreeNode* root) {
        maxGain(root);
        return ans;
    }
};