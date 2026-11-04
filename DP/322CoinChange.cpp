// 给定一个整数数组 coins 表示不同面额的硬币，和一个整数 amount 表示总金额。
// 返回凑成 amount 所需的最少硬币数。如果凑不出来，返回 -1。 每种硬币可以无限次使用。
// 示例：
// 输入：coins = [1, 2, 5], amount = 11
// 输出：3
// 解释：5 + 5 + 1 = 11
// 输入：coins = [2], amount = 3
// 输出：-1
// 解释：只有面额 2，凑不出 3
// 输入：coins = [1], amount = 0
// 输出：0
//
// 动态规划。定义 dp[i] = 凑出金额 i 所需的最少硬币数。
// 对于每个金额 i，尝试每种硬币 c：如果 i - c >= 0 且 dp[i - c] 有解，
// 那么 dp[i] = min(dp[i], dp[i - c] + 1)。
// 意思就是：要凑出 i，可以先凑出 i - c，然后再加一枚面额 c 的硬币。在所有硬币中选最优的。

#include <vector>
using namespace std;
int coinChange(vector<int> coins, int amount) {
    vector<int> dp(amount + 1, amount + 1); // Initialize dp array with amount + 1 (impossible value)
    dp[0] = 0; // Base case: 0 coins needed to make amount 0

    for (int i = 1; i <= amount; ++i) {
        for (int coin : coins) {
            if (i - coin >= 0) {
                dp[i] = min(dp[i], dp[i - coin] + 1);
            }
        }
    }
    return dp[amount] > amount ? -1 : dp[amount]; // If dp[amount] is still the impossible value, return -1
}
