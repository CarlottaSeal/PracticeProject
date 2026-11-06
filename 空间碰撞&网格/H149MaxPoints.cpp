//给定一个二维平面上的 n 个点 points，其中 points[i] = [xi, yi]，找出在同一条直线上的最多点数。
// 示例：
// 输入：points = [[1,1],[2,2],[3,3]]
// 输出：3
//
// 输入：points = [[1,1],[3,2],[5,3],[4,1],[2,3],[1,4]]
// 输出：4
// 1 <= points.length <= 300
// points[i].length == 2
// -10⁴ <= xi, yi <= 10⁴
// 所有点互不相同
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <cstdlib>
#include <numeric>
using namespace std;

int maxPoints(vector<vector<int>>& points) {
    int n = points.size();
    if (n <= 2) return n;

    int maxCount = 2;
    for (int i = 0; i < n; ++i) {
        map<pair<int, int>, int> slopeCount;
        for (int j =i+1; j < n; ++j) {
            int dx = points[j][0] - points[i][0];
            int dy = points[j][1] - points[i][1];
            int g = gcd(dx, dy);  // greatest common divisor，std::gcd自动返回非负结果
            dx /= g;
            dy /= g;
            if (dx < 0) {
                dx = -dx;
                dy = -dy;
            } else if (dx == 0) {
                dy = abs(dy); // vertical line
            }
            slopeCount[{dx, dy}]++;
            maxCount = max(maxCount, slopeCount[{dx, dy}] + 1); // +1 for the point itself
        }
    }
    return maxCount;
}

// unordered_map version: pair has no std::hash, so pack (dx, dy) into one int
// dx in [0, 2e4], dy + 20000 in [0, 4e4] -> base 40001, max ~8e8 fits in int
int maxPointsHash(vector<vector<int>>& points) {
    int n = points.size();
    if (n <= 2) return n;

    int maxCount = 2;
    for (int i = 0; i < n; ++i) {
        unordered_map<int, int> slopeCount;
        for (int j = i + 1; j < n; ++j) {
            int dx = points[j][0] - points[i][0];
            int dy = points[j][1] - points[i][1];
            int g = gcd(dx, dy);
            dx /= g;
            dy /= g;
            if (dx < 0 || (dx == 0 && dy < 0)) {
                dx = -dx;
                dy = -dy;
            }
            int key = dx * 40001 + (dy + 20000);
            int c = ++slopeCount[key];
            maxCount = max(maxCount, c + 1);
        }
    }
    return maxCount;
}