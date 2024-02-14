#include <iostream>
#include <list>
#include <map>
#include <fstream>
#include "profile.h"

void print_order(const std::pair<int, int>* numbers, const int n) {
	{
		LOG_DURATION("function time")
		if (n == 0) {
			std::cout << "empty array of pairs\n";
			return;
		}
		std::map<int, std::list<int>::iterator> idx;
		std::list<int> lst;
		lst.push_back(numbers[0].first);
		idx[numbers[0].first] = lst.begin();
		for (int i = 1; i < n; ++i) {
			if (idx.find(numbers[i].second) != idx.end()) //exist
				idx[numbers[i].first] = lst.insert(idx[numbers[i].second], numbers[i].first);
			else //not exist
				idx[numbers[i].first] = lst.insert(lst.end(), numbers[i].first);
		}

		for (auto i : lst)
			std::cout << i << '\n';
	}
}
int main(){
	int n = 0;

	std::cin >> n;
	std::pair<int, int>* numbers = new std::pair<int, int>[n] {};
	int fst = 0, snd = 0;
	for (int i = 0; i < n; ++i)
	std::cin >> numbers[i].first >> numbers[i].second;
	print_order(numbers, n);
	std::ifstream in("file.txt"); // окрываем файл для чтения
	if (in.is_open())
	{
		in >> n;
		std::pair<int, int>* numbers = new std::pair<int, int>[n] {};
		int fst = 0, snd = 0;
		for (int i = 0; i < n; ++i)
			in >> numbers[i].first >> numbers[i].second;

		print_order(numbers, n);
	}
	return 0;
}