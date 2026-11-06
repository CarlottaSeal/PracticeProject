/*
 * LeetCode 341：扁平化嵌套列表迭代器
 *
 * 给定一个嵌套整数列表 nestedList。列表中的每个元素 NestedInteger 可能
 * 保存一个整数，也可能保存另一个由 NestedInteger 组成的列表，嵌套层数
 * 不受限制。
 *
 * 请实现 NestedIterator，将该嵌套结构按从左到右的深度优先顺序扁平化：
 * - next() 返回扁平序列中的下一个整数；
 * - hasNext() 判断序列中是否仍有可返回的整数。
 *
 * 测试程序会交替调用 hasNext() 和 next()，直到 hasNext() 返回 false；
 * 最终得到的整数顺序应与原嵌套列表从左到右展开后的顺序一致。
 */
#include <vector>
#include <queue>
#include <stack>
using namespace std;

// LeetCode 给的接口，本地编译用的最小实现
class NestedInteger {
	bool isInt;
	int val = 0;
	vector<NestedInteger> list;

public:
	NestedInteger(int v) : isInt(true), val(v) {}
	NestedInteger(vector<NestedInteger> l) : isInt(false), list(move(l)) {}

	bool isInteger() const { return isInt; }
	int getInteger() const { return val; }
	const vector<NestedInteger>& getList() const { return list; }
};

//1.构造时递归展开
class NestedIterator {
	vector<int> vals;
	int idx = 0;
	
	void flatten(const vector<NestedInteger>& nestedList) {
		for (const auto& ni : nestedList) {
			if (ni.isInteger()) {
				vals.push_back(ni.getInteger());
			} else {
				flatten(ni.getList());
			}
		}
	}

public:
	NestedIterator(vector<NestedInteger>& nestedList) {
		flatten(nestedList);
	}

	int next() {
		return vals[idx++];
	}
	bool hasNext() {
		return idx < vals.size();
	}
};
//2.构造时迭代展开*
class NestedIterator {
	stack<NestedInteger> stk;

public:
	NestedIterator(vector<NestedInteger>& nestedList) {
		for (int i = (int)nestedList.size() - 1; i >= 0; --i) {
			stk.push(nestedList[i]);
		}
	}

	int next() {
		hasNext();   // 确保栈顶是整数
		int value = stk.top().getInteger();
		stk.pop();
		return value;
	}
	bool hasNext() {
		while (!stk.empty() && !stk.top().isInteger()) {
			auto list = stk.top().getList(); //只对要处理的类型压栈
			stk.pop();
			for (int i = (int)list.size() - 1; i >= 0; --i) {
				stk.push(list[i]);
			}
		}
		return !stk.empty();
	}
};