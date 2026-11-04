// 实现一个支持快照功能的数组 SnapshotArray：
// SnapshotArray(int length)：初始化一个长度为 length 的数组，所有元素初始为 0
// void set(int index, int val)：将 index 位置的值设为 val
// int snap()：拍一个快照，返回快照 id（从 0 开始递增）
// int get(int index, int snap_id)：返回在指定快照时，index 位置的值【一个snap只可能一次set是吗】
//重点就是 get 能查历史状态，而不是当前值。snap 之后再怎么改，之前拍的快照不受影响。
// 举个例子，index = 0 这个位置的记录可能长这样：
// [(0, 5), (3, 8), (7, 2)]
// 意思是：
// 快照 0 的时候，值是 5
// 快照 3 的时候，被改成了 8
// 快照 7 的时候，被改成了 2

#include <unordered_map>
using namespace std;

class SnapshotArray
{
    int curSnap;
    vector<vector<pair<int,int>>> data;  // data[index] = [(snap_id, val), ...]

public:
    SnapshotArray(int length) : curSnap(0), data(length)
    {
        //每个位置初始化一条记录：snap 0时值为0
        for (int i = 0; i < length; ++i)
        {
            data[i].emplace_back(0, 0); //emplace_back 是直接把参数转发给构造函数，所以不需要大括号，直接传两个参数
            //push_back 是先构造好一个对象再塞进去，所以需要大括号来构造 pair
        }
    }

    void set(int index, int val)
    {   // 同一个 snap 内多次 set，只保留最新的
        if (data[index].back().first == curSnap) //first: snap_id 
        {
            data[index].back().second = val;
        }
        else
        {
            data[index].emplace_back(curSnap, val);
        }
    }
    int snap()
    {
        return curSnap++;
    }
    int get(int index, int snap_id)
    {
        auto& records = data[index];
        //二分查找第一个 snap_id > snap_id 的记录，答案是它前一个
        int left = 0, right = records.size();
        while (left < right)
        {
            int mid = left + (right - left) / 2;
            if (records[mid].first <= snap_id)
            {
                left = mid + 1;
            }
            else
            {
                right = mid;
            }
        }
        return records[left - 1].second; //records[0] 是 (0,0)，left 至少为 1
    }
};
