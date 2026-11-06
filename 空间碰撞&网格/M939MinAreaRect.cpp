/*
 * LeetCode 939：最小面积矩形
 *
 * 给定一个二维整数数组 points，其中 points[i] = [x_i, y_i] 表示平面上的
 * 一个点。所有点互不相同。
 *
 * 请从这些点中选择四个点，使它们组成一个边与坐标轴平行的矩形，并返回
 * 所有可组成矩形中的最小面积。矩形的边长必须大于 0；如果不存在满足条件
 * 的矩形，则返回 0。
 */
 //枚举对角线：x1!= x2 && y1!=y2，检查(x1,y2)和(x2,y1)是否存在[构成矩形的必要条件]
//使用hash set存储点的坐标，检查点是否存在的时间复杂度为O(1)
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
using namespace std;

struct vec2 {
	int x, y;
	vec2(int x, int y) : x(x), y(y) {}
	bool operator==(const vec2& other) const {
		return x == other.x && y == other.y;
	}
};
int minAreaRect(vector<vec2>& points) {
	unordered_map<int, unordered_set<int>> xToYs;
	for (auto& p : points)
	{
		xToYs[p.x].insert(p.y);
	}
	int minArea = INT_MAX;
	for (int i = 0; i < points.size(); ++i)
	{
		for (int j = i + 1; j < points.size(); ++j)
		{
			int x1 = points[i].x, y1 = points[i].y;
			int x2 = points[j].x, y2 = points[j].y;

			if (x1 != x2 && y1 != y2) {
				if (xToYs[x1].contains(y2) && xToYs[x2].contains(y1)) {
					int area = abs(x1 - x2) * abs(y1 - y2);
					minArea = min(minArea, area);
				}
			}
		}
	}
	return minArea == INT_MAX ? 0 : minArea;
}