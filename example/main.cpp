#include <iostream>
#include <algorithm>
#include <assert.h>
#include "skiplist.h"

#define TEST_SKIP_LIST_SIZE 9

//顺序从尾部插入
void TestInsertTail(skiplist::SkipList<int, int>& skip_list) {
	for (int i = 1; i <= TEST_SKIP_LIST_SIZE; i++) {
		auto sort_field = i;
		auto value = i;

		bool suc = skip_list.insert(sort_field, value);
		assert(suc);
		assert(skip_list.size() == i);

		int this_rank = 0;
		auto search_result = skip_list.find(sort_field, &this_rank);
		assert(search_result != nullptr);
		assert(this_rank == i);
		assert(skip_list.back() == search_result);
		assert(skip_list.at(i) == search_result);
	}

	int i = 1;
	for (auto node = skip_list.begin(); node != nullptr; node = node->next(), ++i) {
		int this_rank = 0;
		auto search_result = skip_list.find(node->sort_field_, &this_rank);
		assert(search_result != nullptr);
		assert(this_rank == i);
	}

	skip_list.clear();
	assert(skip_list.size() == 0);
	assert(skip_list.begin() == nullptr);
	assert(skip_list.back() == nullptr);
	assert(skip_list.max_level() == 0);
	assert(skip_list.at(0) == nullptr);
	assert(skip_list.at(1) == nullptr);
}

//顺序从头部插入
void TestInsertHead(skiplist::SkipList<int, int>& skip_list) {
	for (int i = TEST_SKIP_LIST_SIZE; i >= 1; i--) {
		auto sort_field = i;
		auto value = i;
		bool suc = skip_list.insert(sort_field, value);
		assert(suc);
		assert(skip_list.size() == TEST_SKIP_LIST_SIZE + 1 - i);

		int this_rank = 0;
		auto search_result = skip_list.find(sort_field, &this_rank);
		assert(search_result != nullptr);
		assert(this_rank == 1);
		assert(skip_list.at(1) == search_result);
	}

	int i = 1;
	for (auto node = skip_list.begin(); node != nullptr; node = node->next(), ++i) {
		int this_rank = 0;
		auto search_result = skip_list.find(node->sort_field_, &this_rank);
		assert(search_result != nullptr);
		assert(this_rank == i);
	}

	skip_list.clear();
	assert(skip_list.size() == 0);
	assert(skip_list.begin() == nullptr);
	assert(skip_list.back() == nullptr);
	assert(skip_list.max_level() == 0);
	assert(skip_list.at(0) == nullptr);
	assert(skip_list.at(1) == nullptr);
}

//乱序插入
void TestInsertRandom(skiplist::SkipList<int, int>& skip_list) {
	std::vector<int> vSort;
	for (int i = 1; i <= TEST_SKIP_LIST_SIZE; i++) {
		vSort.push_back(i);
	}

	std::random_shuffle(vSort.begin(), vSort.end());

	for (auto itr = vSort.begin(); itr != vSort.end(); ++itr) {
		auto sort_field = *itr;
		auto value = *itr;
		bool suc = skip_list.insert(sort_field, value);
		assert(suc);
	}

	assert(skip_list.size() == TEST_SKIP_LIST_SIZE);

	auto node = skip_list.begin();
	assert(node != nullptr && node->value_ == 1);

	node = skip_list.back();
	assert(node != nullptr && node->value_ == TEST_SKIP_LIST_SIZE);

	int i = 1;
	for (node = skip_list.begin(); node != nullptr; node = node->next(), i++) {
		assert(node->value_ == i);
		assert(skip_list.at(i) == node);
	}

	i = 1;
	for (node = skip_list.begin(); node != nullptr; node = node->next(), ++i) {
		int this_rank = 0;
		auto search_result = skip_list.find(node->sort_field_, &this_rank);
		assert(search_result != nullptr);
		assert(this_rank == i);
	}

	skip_list.clear();
	assert(skip_list.size() == 0);
	assert(skip_list.begin() == nullptr);
	assert(skip_list.back() == nullptr);
	assert(skip_list.max_level() == 0);
	assert(skip_list.at(0) == nullptr);
	assert(skip_list.at(1) == nullptr);
}

void CreateSkipList(skiplist::SkipList<int, int>& skip_list) {
	std::vector<int> vSort;
	for (int i = 1; i <= TEST_SKIP_LIST_SIZE; i++) {
		vSort.push_back(i);
	}

	for (auto itr = vSort.begin(); itr != vSort.end(); ++itr) {
		auto sort_field = *itr;
		auto value = *itr;
		bool suc = skip_list.insert(sort_field, value);
		assert(suc);
	}

	assert(skip_list.size() == TEST_SKIP_LIST_SIZE);
	assert(skip_list.begin() != nullptr);
	assert(skip_list.back() != nullptr);
	assert(skip_list.max_level() > 0);
	assert(skip_list.at(0) == nullptr);
	assert(skip_list.at(1) == skip_list.begin());
	assert(skip_list.at(TEST_SKIP_LIST_SIZE) == skip_list.back());
}

//从头部删除
void TestRemoveFromHead(skiplist::SkipList<int, int>& skip_list) {
	CreateSkipList(skip_list);
	for (int i = 1; i <= TEST_SKIP_LIST_SIZE; i++) {
		auto node = skip_list.begin();
		assert(node != nullptr);
		assert(node->value_ == i);
		assert(skip_list.at(1) == node);

		auto sort_field = node->sort_field_;
		bool suc = skip_list.remove(sort_field);
		assert(suc);
		assert(skip_list.size() == TEST_SKIP_LIST_SIZE - i);

		int m = 1;
		for (node = skip_list.begin(); node != nullptr; node = node->next(), ++m) {
			int this_rank = 0;
			auto search_result = skip_list.find(node->sort_field_, &this_rank);
			assert(search_result != nullptr);
			assert(this_rank == m);
		}
	}

	assert(skip_list.size() == 0);
	assert(skip_list.begin() == nullptr);
	assert(skip_list.back() == nullptr);
	assert(skip_list.max_level() == 0);
	assert(skip_list.at(0) == nullptr);
	assert(skip_list.at(1) == nullptr);
}

//从尾部删除
void TestRemoveFromTail(skiplist::SkipList<int, int>& skip_list) {
	CreateSkipList(skip_list);
	for (int i = TEST_SKIP_LIST_SIZE; i >= 1; i--) {
		auto node = skip_list.begin();
		assert(node != nullptr);
		assert(node->value_ == 1);
		assert(skip_list.at(1) == node);

		node = skip_list.back();
		assert(node != nullptr);
		assert(node->value_ == i);
		assert(skip_list.at(i) == node);

		auto sort_field = node->sort_field_;
		bool suc = skip_list.remove(sort_field);
		assert(suc);
		assert(skip_list.size() == i - 1);

		int m = 1;
		for (node = skip_list.begin(); node != nullptr; node = node->next(), ++m) {
			int this_rank = 0;
			auto search_result = skip_list.find(node->sort_field_, &this_rank);
			assert(search_result != nullptr);
			assert(this_rank == m);
		}
	}

	assert(skip_list.size() == 0);
	assert(skip_list.begin() == nullptr);
	assert(skip_list.back() == nullptr);
	assert(skip_list.max_level() == 0);
	assert(skip_list.at(0) == nullptr);
	assert(skip_list.at(1) == nullptr);
}

//随机删除
void TestRemoveRandom(skiplist::SkipList<int, int>& skip_list) {
	CreateSkipList(skip_list);

	std::vector<int> vSort;
	for (int i = 1; i <= TEST_SKIP_LIST_SIZE; i++) {
		vSort.push_back(i);
	}

	std::random_shuffle(vSort.begin(), vSort.end());
	for (auto itr = vSort.begin(); itr != vSort.end(); ++itr) {
		auto sort_field = *itr;
		// auto value = *itr;

		bool suc = skip_list.remove(sort_field);
		assert(suc);

		int m = 1;
		for (auto node = skip_list.begin(); node != nullptr; node = node->next(), ++m) {
			int this_rank = 0;
			auto search_result = skip_list.find(node->sort_field_, &this_rank);
			assert(search_result != nullptr);
			assert(this_rank == m);
		}
	}

	assert(skip_list.size() == 0);
	assert(skip_list.begin() == nullptr);
	assert(skip_list.back() == nullptr);
	assert(skip_list.max_level() == 0);
	assert(skip_list.at(0) == nullptr);
	assert(skip_list.at(1) == nullptr);
}

int main() {
	skiplist::SkipList<int, int> skip_list(0, 0);
	std::cout << "skiplist test now starts..." << std::endl;

	TestInsertTail(skip_list);
	TestInsertHead(skip_list);
	TestInsertRandom(skip_list);
	TestRemoveFromHead(skip_list);
	TestRemoveFromTail(skip_list);
	TestRemoveRandom(skip_list);

	CreateSkipList(skip_list);

	//测试遍历与查找
	for (auto node = skip_list.begin(); node != nullptr; node = node->next()) {
		int sort_field = node->sort_field_;
		int rank = 0;
		auto search_result = skip_list.find(sort_field, &rank);
		assert(search_result != nullptr);
	}

	//从头开始删除
	while (skip_list.size() > 0) {
		int sort_field = skip_list.begin()->sort_field_;
		bool removeResult = skip_list.remove(sort_field);
		assert(removeResult);
	}

	assert(skip_list.size() == 0);
	assert(skip_list.begin() == nullptr);
	assert(skip_list.back() == nullptr);
	assert(skip_list.max_level() == 0);
	assert(skip_list.at(0) == nullptr);
	assert(skip_list.at(1) == nullptr);

	std::cout << "skiplist test now finished..." << std::endl;
	return 0;
}
