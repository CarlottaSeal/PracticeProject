// 设计一个数据结构，支持以下操作，且每个操作的平均时间复杂度都是 O(1)：
// insert(val)：插入元素，已存在则返回 false
// remove(val)：删除元素，不存在则返回 false
// getRandom()：等概率随机返回一个元素

// 哈希表->插入；删除；
// 数组->随机；

class RandomizedSet {
    vector<int> nums;
    unordered_map<int, int> valToIdx; // 值 -> 在 nums 中的下标

public:
    bool insert(int val) {
        if(valToIdx.contains(val)) return false;    //已经有了

        valToIdx[val] = nums.size();
        nums.push_back(val);
        return true;
    }

    bool remove(int val) {
        if(!valToIdx.contains(val)) return false;

        int index = valToIdx[val];
        nums[index] = nums.back();
        valToIdx.erase(val);
        valToIdx[nums[index]] = index;
        nums.pop_back();
        return true;
    }

    int getRandom() {
        return nums[rand() % nums.size()];
    }
};