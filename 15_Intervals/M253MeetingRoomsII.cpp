/*
 * LeetCode 253：会议室 II
 *
 * 给定一组会议时间 intervals，其中 intervals[i] = [start_i, end_i] 表示
 * 第 i 场会议从 start_i 开始，到 end_i 结束。每个会议室在同一时刻只能
 * 举行一场会议；一场会议结束后，该会议室可以立即用于另一场会议。
 *
 * 请计算为了安排完所有会议，至少需要同时准备多少个会议室。
 */
#include <vector>
#include <algorithm>
#include <functional>
#include <queue>
using namespace std;
//1. 扫描栈
int minMeetingRooms(vector<vector<int>>& intervals) {
	vector<pair<int, int>> times;
	for (const auto& interval : intervals) {
		times.emplace_back(interval[0], 1);  // start time
		times.emplace_back(interval[1], -1); // end time
	}
	sort(times.begin(), times.end());

	int maxRooms = 0, currentRooms = 0;
	for (const auto& time : times) {
		currentRooms += time.second;
		maxRooms = max(maxRooms, currentRooms);
	}
	return maxRooms;
}

//2. 排序+最小堆
int minMeetingRoomsB(vector<vector<int>>& intervals) {
	sort(intervals.begin(), intervals.end());
	priority_queue<int, vector<int>, greater<int>> pq;
	for (auto& div : intervals)
	{
		if (!pq.empty() && pq.top() <= div[0]) {
			pq.pop();
		}
		pq.push(div[1]);
	}
	return pq.size();
// 因为堆里每个元素代表一个正在使用的会议室。
// 走一遍逻辑：
// 遍历每个会议（按开始时间排好序的）
// pq.top() <= interval[0]：堆顶是最早结束的会议室，如果它在新会议开始前就结束了，说明这个房间空出来了，pop 掉，新会议复用这个房间
// 如果堆顶还没结束（或者堆为空），新会议就只能开一间新房间
// 不管哪种情况，最后都 push 新会议的结束时间进去
//
// 所以堆的大小始终等于"当前同时进行的会议数"。整个过程中堆的最大大小就是答案，但因为你是按开始时间顺序遍历的，能释放的房间都及时释放了，遍历结束时堆里剩下的就是同时需要的最多房间数。
}