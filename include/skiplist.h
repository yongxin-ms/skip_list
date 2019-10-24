#pragma once
#include <iostream>
#include "skipnode.h"
#include "random.h"

namespace skiplist {
	template<typename SortField, typename Value>
	class SkipList {
	public:
		SkipList();
		~SkipList();

		const Node<SortField, Value>* find(const SortField& sort_field, int* rank = nullptr) const;
		const Node<SortField, Value>* at(int rank) const;
		const Node<SortField, Value>* begin() const;
		const Node<SortField, Value>* back() const;
		bool insert(const SortField& sort_field, const Value& value);
		bool remove(const SortField& sort_field);
		void clear();

		int max_level() const { return max_level_; };
		int size() const { return node_count_; };

	private:
		Node<SortField, Value>* CreateNode(const SortField& sort_field, const Value& value) const;
		static int CreateRandomLevel();

	private:
		int max_level_;						//从1开始
		int node_count_;					//节点数量，不包括头节点和尾节点
		Node<SortField, Value>* header_;
		Node<SortField, Value>* footer_;
		static const int MAX_LEVEL = 32;	//最高层数，支持42亿个元素Log(N)的时间复杂度
	};

	template<typename SortField, typename Value>
	SkipList<SortField, Value>::SkipList() : max_level_(0), node_count_(0) {
		header_ = new Node<SortField, Value>(MAX_LEVEL, SortField(), Value());
		footer_ = new Node<SortField, Value>(0, SortField(), Value());
		for (int i = 0; i < MAX_LEVEL; i++)
			header_->level_[i].forward = footer_;
	}

	template<typename SortField, typename Value>
	SkipList<SortField, Value>::~SkipList() {
		clear();
		delete header_;
		delete footer_;
	}

	template<typename SortField, typename Value>
	const Node<SortField, Value>* SkipList<SortField, Value>::find(const SortField& sort_field, int* rank) const {
		if (rank != nullptr)
			*rank = 1;
		auto node = header_;
		for (int i = max_level_ - 1; i >= 0; --i) {
			//找到目标节点的前节点
			while (node->level_[i].forward != footer_ &&
			node->level_[i].forward->sort_field_ < sort_field) {
				if (rank != nullptr)
					*rank += node->level_[i].span;
				node = node->level_[i].forward;
			}
		}

		//如果该跳跃表为空，头节点就会直接指向尾节点
		if (node == footer_)
			return nullptr;
		node = node->level_[0].forward;
		if (node == footer_)
			return nullptr;
		if (node->sort_field_ != sort_field)
			return nullptr;
		return node;
	}

	//获取排行榜第rank名，从1开始
	template<typename SortField, typename Value>
	const Node<SortField, Value>* SkipList<SortField, Value>::at(int rank) const {
		if (rank <= 0 || rank > node_count_)
			return nullptr;

		int cur_rank = 0;
		auto node = header_;
		for (int i = max_level_ - 1; i >= 0; --i) {
			//找到目标节点的前节点
			while (node->level_[i].forward != footer_ &&
			cur_rank + node->level_[i].span < rank) {
				cur_rank += node->level_[i].span;
				node = node->level_[i].forward;
			}
		}

		//如果该跳跃表为空，头节点就会直接指向尾节点
		if (node == footer_)
			return nullptr;
		cur_rank += node->level_[0].span;
		node = node->level_[0].forward;
		if (node == footer_)
			return nullptr;
		if (cur_rank != rank)
			return nullptr;
		return node;
	}

	template<typename SortField, typename Value>
	const Node<SortField, Value>* SkipList<SortField, Value>::begin() const {
		return header_->level_[0].forward == footer_ ? nullptr : header_->level_[0].forward;
	}

	//获取最后一个节点
	template<typename SortField, typename Value>
	const Node<SortField, Value>* SkipList<SortField, Value>::back() const {
		auto node = header_;
		for (int i = max_level_ - 1; i >= 0; --i) {
			while (node->level_[i].forward != footer_) {
				node = node->level_[i].forward;
			}
		}

		//如果该跳跃表为空，头结点就是尾结点前面一个
		//感谢zoey发现这个bug, 20190807
		if (node == header_)
			return nullptr;
		return node;
	}

	template<typename SortField, typename Value>
	bool SkipList<SortField, Value>::insert(const SortField& sort_field, const Value& value) {
		Node<SortField, Value>* update[MAX_LEVEL];
		int span[MAX_LEVEL];

		auto node = header_;
		for (int i = max_level_ - 1; i >= 0; --i) {
			//span[i-1]用来记录第i层达到插入位置的跨度,也是该层最接近(小于)给定score的排名
			//span[i-1]初始化为上一层所跨越的节点总数,因为上一层已经加过
			span[i] = (i == (max_level_ - 1) ? 0 : span[i+1]);

			while (node->level_[i].forward != footer_ &&
			node->level_[i].forward->sort_field_ < sort_field) {
				span[i] += node->level_[i].span;
				node = node->level_[i].forward;
			}
			update[i] = node;
		}

		node = node->level_[0].forward;

		//如果key已存在
		if (node != footer_ && node->sort_field_ == sort_field) {
			return false;
		}

		auto newNode = CreateNode(sort_field, value);
		if (newNode->node_level_ > max_level_) {
			//每次最多增加一层
			max_level_ = newNode->node_level_;
			span[max_level_ - 1] = 0;
			update[max_level_ - 1] = header_;
			update[max_level_ - 1]->level_[max_level_ - 1].span = size();
		}

		//调整forward指针
		for (int i = 0; i < newNode->node_level_ ; ++i) {
			node = update[i];
			newNode->level_[i].forward = node->level_[i].forward;
			node->level_[i].forward = newNode;

			newNode->level_[i].span = node->level_[i].span - (span[0] - span[i]);
			node->level_[i].span = span[0] - span[i] + 1;
		}

		for (int i = max_level_ - 1; i >= newNode->node_level_; --i) {
			update[i]->level_[i].span++;
		}

		++node_count_;
		return true;
	}

	template<typename SortField, typename Value>
	bool SkipList<SortField, Value>::remove(const SortField& sort_field) {
		//保存的是要删除的前一个节点
		Node<SortField, Value>* update[MAX_LEVEL];
		auto node = header_;
		for (int i = max_level_ - 1; i >= 0; --i) {
			while (node->level_[i].forward != footer_ &&
			node->level_[i].forward->sort_field_ < sort_field) {
				node = node->level_[i].forward;
			}
			update[i] = node;
		}

		if (node == footer_)
			return false;
		node = node->level_[0].forward;
		if (node == footer_)
			return false;
		//如果节点不存在
		if (node->sort_field_ != sort_field) {
			return false;
		}

		//现在node已经被找到，将要被删除，
		for (int i = 0; i <= max_level_ - 1; ++i) {
			if (update[i]->level_[i].forward != node) {
				//跳过了将要被删除的节点，所以跨度直接-1就好
				update[i]->level_[i].span--;
			} else {
				//连接着将要被删除的节点，需要改一下指针，更新一下跨度
				update[i]->level_[i].forward = node->level_[i].forward;
				update[i]->level_[i].span += (node->level_[i].span - 1);
			}
		}
		delete node;

		//更新max_level_的值，因为有可能在移除一个节点之后，max_level_值会发生变化，
		//及时降低可提高性能
		while (max_level_ > 0 &&
		header_->level_[max_level_ - 1].forward == footer_) {
			//如果头结点连着尾节点，这个高度是浪费的
			--max_level_;
		}

		--node_count_;
		return true;
	}

	//清空跳跃表，注意头节点和尾节点会保留
	template<typename SortField, typename Value>
	void SkipList<SortField, Value>::clear() {
		auto p = header_->level_[0].forward;
		while (p != footer_) {
			auto q = p->level_[0].forward;
			delete p;
			p = q;
		}
		for (int i = 0; i < MAX_LEVEL; i++) {
			header_->level_[i].forward = footer_;
			header_->level_[i].span = 0;
		}
		node_count_ = 0;
		max_level_ = 0;
	}

	template<typename SortField, typename Value>
	Node<SortField, Value>* SkipList<SortField, Value>::CreateNode(const SortField& sort_field, const Value& value) const {
		int nodeLevel = CreateRandomLevel();
		if (node_count_ == 0) {
			nodeLevel = 1;
		}
		else if (nodeLevel > max_level_ + 1) {
			nodeLevel = max_level_ + 1;
		}
		auto node = new Node<SortField, Value>(nodeLevel, sort_field, value);
		return node;
	}

	//随机生成一个level
	template<typename SortField, typename Value>
	int SkipList<SortField, Value>::CreateRandomLevel() {
		int level = 1;
		while (level < MAX_LEVEL && Random::RandomInt(0, 1) == 0) {
			level++;
		}
		return level;
	}
}
