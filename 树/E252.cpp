//给定一组会议的起止时间 intervals[i] = [start, end]，判断一个人能否参加所有会议（即会议之间是否有重叠）。

class Solution {
public:
    bool canAttendMeetings(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end());
        for (int i = 1; i < intervals.size(); i++) {
            if (intervals[i][0] < intervals[i - 1][1])
                return false;
        }
        return true;
    }
};