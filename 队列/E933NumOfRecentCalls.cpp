//题意：实现 RecentCounter，每次 ping(t) 返回 [t-3000, t] 范围内的请求数。
// 最简单的一个queue

class RecentCounter {
    queue<int> q;
public:
    int ping(int t) {
        q. push(t);
        while(q.front < t - 3000) q.pop();

        return q.size();
    }
};