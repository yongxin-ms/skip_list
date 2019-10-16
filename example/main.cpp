#include <iostream>
#include <algorithm>
#include "skiplist.h"

#define TEST_SKIP_LIST_SIZE 9

//顺序从尾部插入
void TestInsertTail(skiplist::SkipList<int, int>& skip_list) {
	for (int i = 1; i <= TEST_SKIP_LIST_SIZE; i++) {
		auto sort_field = i;
		auto value = i;
		bool suc = skip_list.insert(sort_field, value);
		if (!suc) {
			std::cout << "error" << std::endl;
		}

		if (skip_list.size() != i) {
			std::cout << "error" << std::endl;
		}

		int this_rank = 0;
		const skiplist::Node<int, int>* searchResult = skip_list.find(sort_field, &this_rank);
		if (searchResult == nullptr) {
			std::cout << "error" << std::endl;
		}

		if (this_rank != i) {
			std::cout << "error" << std::endl;
		}

		if (searchResult != skip_list.back()) {
			std::cout << "error" << std::endl;
		}

		if (skip_list.at(i) != searchResult) {
			std::cout << "error" << std::endl;
		}
	}

	int i = 1;
	for (auto it = skip_list.begin(); it != nullptr; it = it->next(), ++i) {
		int this_rank = 0;
		const skiplist::Node<int, int>* searchResult = skip_list.find(it->sort_field_, &this_rank);
		if (searchResult == nullptr) {
			std::cout << "error" << std::endl;
		}

		if (this_rank != i) {
			std::cout << "error" << std::endl;
		}
	}

	skip_list.clear();
}

//顺序从头部插入
void TestInsertHead(skiplist::SkipList<int, int>& skip_list) {
	for (int i = TEST_SKIP_LIST_SIZE; i >= 1; i--) {
		auto sort_field = i;
		auto value = i;
		bool suc = skip_list.insert(sort_field, value);
		if (!suc) {
			std::cout << "error" << std::endl;
		}

		if (skip_list.size() != TEST_SKIP_LIST_SIZE + 1 - i) {
			std::cout << "error" << std::endl;
		}

		int this_rank = 0;
		const skiplist::Node<int, int>* searchResult = skip_list.find(sort_field, &this_rank);
		if (searchResult == nullptr) {
			std::cout << "error" << std::endl;
		}

		if (this_rank != 1) {
			std::cout << "error" << std::endl;
		}

		if (skip_list.at(1) != searchResult) {
			std::cout << "error" << std::endl;
		}
	}

	int i = 1;
	for (auto it = skip_list.begin(); it != nullptr; it = it->next(), ++i) {
		int this_rank = 0;
		const skiplist::Node<int, int>* searchResult = skip_list.find(it->sort_field_, &this_rank);
		if (searchResult == nullptr) {
			std::cout << "error" << std::endl;
		}

		if (this_rank != i) {
			std::cout << "error" << std::endl;
		}
	}

	skip_list.clear();
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
	}

	if (skip_list.size() != TEST_SKIP_LIST_SIZE) {
		std::cout << "error" << std::endl;
	}

	auto node = skip_list.begin();
	if (node == nullptr || node->value_ != 1) {
		std::cout << "error" << std::endl;
	}

	node = skip_list.back();
	if (node == nullptr || node->value_ != TEST_SKIP_LIST_SIZE) {
		std::cout << "error" << std::endl;
	}

	int i = 1;
	for (node = skip_list.begin(); node != nullptr; node = node->next(), i++) {
		if (node->value_ != i) {
			std::cout << "error" << std::endl;
		}

		if (skip_list.at(i) != node) {
			std::cout << "error" << std::endl;
		}
	}

	i = 1;
	for (auto it = skip_list.begin(); it != nullptr; it = it->next(), ++i) {
		int this_rank = 0;
		const skiplist::Node<int, int>* searchResult = skip_list.find(it->sort_field_, &this_rank);
		if (searchResult == nullptr) {
			std::cout << "error" << std::endl;
		}

		if (this_rank != i) {
			std::cout << "error" << std::endl;
		}
	}

	skip_list.clear();
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
	}
}

//从头部删除
void TestRemoveFromHead(skiplist::SkipList<int, int>& skip_list) {
	CreateSkipList(skip_list);
	for (int i = 1; i <= TEST_SKIP_LIST_SIZE; i++) {
		auto node = skip_list.begin();
		if (node == nullptr) {
			std::cout << "error" << std::endl;
			break;
		}

		if (node->value_ != i) {
			std::cout << "error" << std::endl;
		}

		if (skip_list.at(1) != node) {
			std::cout << "error" << std::endl;
		}

		auto sort_field = node->sort_field_;
		bool suc = skip_list.remove(sort_field);
		if (!suc) {
			std::cout << "error" << std::endl;
		}

		if (skip_list.size() != TEST_SKIP_LIST_SIZE - i) {
			std::cout << "error" << std::endl;
		}

		int m = 1;
		for (auto it = skip_list.begin(); it != nullptr; it = it->next(), ++m) {
			int this_rank = 0;
			const skiplist::Node<int, int>* searchResult = skip_list.find(it->sort_field_, &this_rank);
			if (searchResult == nullptr) {
				std::cout << "error" << std::endl;
			}

			if (this_rank != m) {
				std::cout << "error" << std::endl;
			}
		}
	}

	if (skip_list.size() != 0) {
		std::cout << "error" << std::endl;
	}
}

//从尾部删除
void TestRemoveFromTail(skiplist::SkipList<int, int>& skip_list) {
	CreateSkipList(skip_list);
	for (int i = TEST_SKIP_LIST_SIZE; i >= 1; i--) {
		auto node = skip_list.begin();
		if (node == nullptr) {
			std::cout << "error" << std::endl;
			break;
		}

		if (node->value_ != 1) {
			std::cout << "error" << std::endl;
		}

		if (skip_list.at(1) != node) {
			std::cout << "error" << std::endl;
		}

		node = skip_list.back();
		if (node == nullptr) {
			std::cout << "error" << std::endl;
			break;
		}

		if (node->value_ != i) {
			std::cout << "error" << std::endl;
		}

		if (skip_list.at(i) != node) {
			std::cout << "error" << std::endl;
		}

		auto sort_field = node->sort_field_;
		bool suc = skip_list.remove(sort_field);
		if (!suc) {
			std::cout << "error" << std::endl;
		}

		if (skip_list.size() != i - 1) {
			std::cout << "error" << std::endl;
		}

		int m = 1;
		for (auto it = skip_list.begin(); it != nullptr; it = it->next(), ++m) {
			int this_rank = 0;
			const skiplist::Node<int, int>* searchResult = skip_list.find(it->sort_field_, &this_rank);
			if (searchResult == nullptr) {
				std::cout << "error" << std::endl;
			}

			if (this_rank != m) {
				std::cout << "error" << std::endl;
			}
		}
	}

	if (skip_list.size() != 0) {
		std::cout << "error" << std::endl;
	}
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
		auto value = *itr;
		
		bool suc = skip_list.remove(sort_field);
		if (!suc) {
			std::cout << "error" << std::endl;
		}

		int m = 1;
		for (auto it = skip_list.begin(); it != nullptr; it = it->next(), ++m) {
			int this_rank = 0;
			const skiplist::Node<int, int>* searchResult = skip_list.find(it->sort_field_, &this_rank);
			if (searchResult == nullptr) {
				std::cout << "error" << std::endl;
			}

			if (this_rank != m) {
				std::cout << "error" << std::endl;
			}
		}
	}

	if (skip_list.size() != 0) {
		std::cout << "error" << std::endl;
	}
}


int main() {
	skiplist::SkipList<int, int> skip_list;
	std::cout << "skiplist test now starts..." << std::endl;

	TestInsertTail(skip_list);
	TestInsertHead(skip_list);
	TestInsertRandom(skip_list);
	TestRemoveFromHead(skip_list);
	TestRemoveFromTail(skip_list);
	TestRemoveRandom(skip_list);

//	skip_list.DumpAllNodes();
// 	skip_list.DumpAllNodes();
// 	skip_list.DumpNodeHight();

	CreateSkipList(skip_list);

	//测试遍历与查找
	for (const skiplist::Node<int, int>* itr = skip_list.begin(); itr != nullptr; itr = itr->next())
	{
		int sort_field = itr->sort_field_;
		int rank = 0;
		const skiplist::Node<int, int>* searchResult = skip_list.find(sort_field, &rank);
		if (searchResult == nullptr) {
			std::cout << "error: search failure, " << sort_field << std::endl;
		}
	}

	//从头开始删除
	while (skip_list.size() > 0)
	{
		int sort_field = skip_list.begin()->sort_field_;
		bool removeResult = skip_list.remove(sort_field);
		if (!removeResult) {
			std::cout << "removed failure, " << sort_field  << std::endl;
		}
	}

	std::cout << "skiplist test now ends..." << std::endl;
    return 0;
}
