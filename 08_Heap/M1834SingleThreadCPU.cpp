/*
 * LeetCode 1834：单线程 CPU
 *
 * 给定任务数组 tasks，其中 tasks[i] = [enqueueTime_i, processingTime_i]：
 * enqueueTime_i 是第 i 个任务可进入等待队列的时刻，processingTime_i 是
 * CPU 完成该任务所需的连续处理时间。CPU 每次只能执行一个任务，并且任务
 * 一旦开始就必须执行到结束。
 *
 * CPU 空闲时，从所有已经进入等待队列的任务中选择处理时间最短的任务；
 * 如果有多个任务的处理时间相同，则选择原数组下标最小的任务。如果等待
 * 队列为空，CPU 将保持空闲，直到有新任务到达。
 *
 * 请返回 CPU 实际执行各任务时对应的原数组下标顺序。
 */
#include <algorithm>
#include <vector>
#include <queue>
#include <array>
using namespace std;
vector<int> getOrder(vector<vector<int>>& tasks)
{
	int n = tasks.size();
	vector<array<int, 3>> sorted(n);
	for (int i = 0; i < n; ++i)
		sorted[i] = { tasks[i][0], tasks[i][1], i };
	sort(sorted.begin(), sorted.end());
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

	vector<int> result;
	long long t = 0; // 修正：原来是 int，累加处理时间会溢出
	int i = 0;
	while (i < n || !pq.empty())
	{
		if (pq.empty() && t < sorted[i][0])
			t = sorted[i][0];
		while (i < n && sorted[i][0] <= t)
		{
			pq.emplace(sorted[i][1], sorted[i][2]);
			++i;
		}
		if (!pq.empty())
		{
			auto [processingTime, index] = pq.top();
			pq.pop();
			t += processingTime;
			result.push_back(index);
		}
	}
	return result;
}