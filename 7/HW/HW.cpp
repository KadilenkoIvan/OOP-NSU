#include "test_runner.h"
#include "profile.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

template <typename String>
using Group = vector<String>;

template <typename String>
using Char = typename String::value_type;

template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
	vector<Group<String>> res;
	if (strings.size() != 0){
		bool flag = true;
		vector<String> tmp = { strings[0] };
		res.push_back(tmp);
		strings.erase(strings.begin());
		for (String i : strings) {
			flag = true;
			string j(i.begin(), i.end());
			sort(j.begin(), j.end());
			j.erase(unique(j.begin(), j.end()), j.end());
			for (int p = 0; p < res.size(); p++) {
				string cmp(res[p][0].begin(), res[p][0].end());
				sort(cmp.begin(), cmp.end());
				cmp.erase(unique(cmp.begin(), cmp.end()), cmp.end());
				if (j == cmp) {
					res[p].push_back(i);
					flag = false;
					break;
				}
			}
			if (flag){
				vector<String> tmp = { i };
				res.push_back(tmp);
			}
		}
	}
	return res;
}

void TestGroupingABC() {
	vector<string> strings = { "caab", "abc", "cccc", "bacc", "c" };
	auto groups = GroupHeavyStrings(strings);
	ASSERT_EQUAL(groups.size(), 2);
	sort(begin(groups), end(groups));    ASSERT_EQUAL(groups[0], vector<string>({ "caab", "abc", "bacc" }));
	ASSERT_EQUAL(groups[1], vector<string>({ "cccc", "c" }));
}

void TestGroupingReal() {
	vector<string> strings = { "law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl" };
	auto groups = GroupHeavyStrings(strings);
	ASSERT_EQUAL(groups.size(), 4);
	sort(begin(groups), end(groups));    ASSERT_EQUAL(groups[0], vector<string>({ "laptop", "paloalto" }));
	ASSERT_EQUAL(groups[1], vector<string>({ "law", "wall", "awl" }));
	ASSERT_EQUAL(groups[2], vector<string>({ "port" }));
	ASSERT_EQUAL(groups[3], vector<string>({ "top", "pot" }));
}

int main() {
	{
		LOG_DURATION("qwerty")
		TestRunner tr;
		RUN_TEST(tr, TestGroupingABC);
		RUN_TEST(tr, TestGroupingReal);
	}
	return 0;
}