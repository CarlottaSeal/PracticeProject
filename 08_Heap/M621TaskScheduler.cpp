/*
 * LeetCode 621：任务调度器
 *
 * 给定一个由大写英文字母组成的任务数组 tasks，以及一个非负整数 n。
 * 每个字母代表一种任务；执行任意任务都恰好占用一个单位时间。任务可以
 * 按任意顺序执行，但两个相同类型的任务之间必须至少间隔 n 个单位时间。
 * 在没有可执行任务时，处理器可以空闲一个单位时间。
 *
 * 请返回完成全部任务所需的最短时间，空闲时间也要计入总时间。
 */
 //1.贪心+数学
//关键在于出现次数最多的任务A maxFreq 若n=2:
// A__ | A__ | A
// AB__|AB__|AB
//∴分成 maxFreq-1 个间隔，每个间隔长度为 n+1，最后一个间隔不需要空闲时间
//∴总时间 = (maxFreq-1)*(n+1) + countMaxFreq  
//但是还有一种情况，任务种类很多，间隔足够大，空闲时间为0（没有idle）
//∴总时间 = max((maxFreq-1)*(n+1) + countMaxFreq, tasks.size())
#include <vector>
#include <queue>
#include <array>
using namespace std;
int leastInterval(vector<char>& tasks, int n) {
	array<int, 26> freq = {};
	for (char task : tasks)
	{
		freq[task - 'A']++;
	}
	int maxFreq = 0;
	for (int f : freq) {
		maxFreq = max(maxFreq, f);
	}
	int countMaxFreq = 0;
	for (int f : freq) {
		if (f == maxFreq) countMaxFreq++;
	}
	return max((maxFreq - 1) * (n + 1) + countMaxFreq, (int)tasks.size());
}

int leastInterval(vector<char>& tasks, int n) {
	// ① 数每种任务出现几次
	array<int, 26> freq = {};
	for (char task : tasks)
		freq[task - 'A']++;

	// ② 把次数放进最大堆
	priority_queue<int> pq;
	for (int f : freq)
		if (f > 0) pq.push(f);

	int time = 0;
	// ③ 每次 while 就是一轮
	while (!pq.empty())
	{
		vector<int> done;   // 这一轮做过、但还没做完的任务
		int count = 0;      // 这一轮实际做了几个任务

		// ④ 这一轮最多做 n+1 个，每次取剩得最多的
		for (int i = 0; i < n+1 && !pq.empty(); i++)
		{
			int f = pq.top();
			pq.pop();
			count ++;
			if (f > 1)
				done.push_back(f - 1);   // 不能马上 push 回 pq，否则同一轮会再取到它，违反冷却
		}

		// ⑤ 一轮结束，把没做完的放回堆
		for (int f : done)
			pq.push(f);

		// ⑥ 计时：不是最后一轮就占满 n+1 格（不够的是 idle）；最后一轮只算实际做的个数
		time += pq.empty() ? count : n+1;
	}
	return time;
}
