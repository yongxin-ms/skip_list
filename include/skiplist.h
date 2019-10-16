﻿#ifndef SKIP_LIST_H_
#define SKIP_LIST_H_

#include <iostream>
#include "skipnode.h"
#include "random.h"

namespace skiplist {
	template<typename SortField, typename Value>
	class SkipList {
	public:
		SkipList() : max_level_(0), node_count_(0) {
			header_ = new Node<SortField, Value>(MAX_LEVEL, SortField(), Value());
			footer_ = new Node<SortField, Value>(0, SortField(), Value());
			for (int i = 0; i < MAX_LEVEL; i++) {
				header_->level_[i].forward = footer_;
			}
		};

		~SkipList() { 
			clear();
			delete header_;
			delete footer_;
		};

		const Node<SortField, Value>* find(const SortField& sort_field, int* rank = nullptr) const;
		const Node<SortField, Value>* at(int rank) const;
		const Node<SortField, Value>* begin() const {
			return header_->level_[0].forward == footer_ ? nullptr : header_->level_[0].forward;
		}
		const Node<SortField, Value>* back() const;

		bool insert(const SortField& sort_field, const Value& value);
		bool remove(const SortField& sort_field);

		int max_level() const { return max_level_; };
		int size() const { return node_count_; };

		//清空跳跃表，注意头节点和尾节点会保留
		void clear() {
			Node<SortField, Value>* p = header_->level_[0].forward;
			while (p != footer_) {
				Node<SortField, Value>* q = p->level_[0].forward;
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

		void DumpAllNodes() const;
		void DumpNodeDetail(const Node<SortField, Value>* node) const;

	private:
		Node<SortField, Value>* CreateNode(const SortField& sort_field, const Value& value) const {
			int nodeLevel = GetRandomLevel();
			if (node_count_ == 0) {
				nodeLevel = 1;
			} else if (nodeLevel > max_level_) {
				nodeLevel = max_level_ + 1;
			}

			Node<SortField, Value>* node = new Node<SortField, Value>(nodeLevel, sort_field, value);
			return node;
		}

		//随机生成一个level
		static int GetRandomLevel() {
			int level = 1;
			while (level < MAX_LEVEL && Random::RandomInt(0, 1) == 0) {
				level++;
			}

			return level;
		}

	private:
		int max_level_;						//从1开始
		int node_count_;					//节点数量，不包括头节点和尾节点
		Node<SortField, Value>* header_;
		Node<SortField, Value>* footer_;

		static const int MAX_LEVEL = 24;	//最多支持1000万的数据量
	};

	template<typename SortField, typename Value>
	const Node<SortField, Value>* SkipList<SortField, Value>::find(const SortField& sort_field, int* rank) const {
		if (rank != nullptr) {
			*rank = 1;
		}

		Node<SortField, Value>* node = header_;
		for (int i = max_level_ - 1; i >= 0; --i) {
			//找到目标节点的前节点
			while (node->level_[i].forward != footer_ && node->level_[i].forward->sort_field_ < sort_field) {
				if (rank != nullptr) *rank += node->level_[i].span;
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
	};

	//获取排行榜第rank名，从1开始
	template<typename SortField, typename Value>
	const Node<SortField, Value>* SkipList<SortField, Value>::at(int rank) const {
		int cur_rank = 0;
		Node<SortField, Value>* node = header_;
		for (int i = max_level_ - 1; i >= 0; --i) {
			//找到目标节点的前节点
			while (node->level_[i].forward != footer_ && cur_rank + node->level_[i].span < rank) {
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

	//获取最后一个节点
	template<typename SortField, typename Value>
	const Node<SortField, Value>* SkipList<SortField, Value>::back() const {
		Node<SortField, Value>* node = header_;
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
		int rank[MAX_LEVEL];

		Node<SortField, Value>* node = header_;
		for (int i = max_level_ - 1; i >= 0; --i) {
			//rank[i-1]用来记录第i层达到插入位置的所跨越的节点总数,也就是该层最接近(小于)给定score的排名  
			//rank[i-1]初始化为上一层所跨越的节点总数,因为上一层已经加过
			rank[i] = (i == (max_level_ - 1) ? 0 : rank[i+1]);

			while (node->level_[i].forward != footer_ && node->level_[i].forward->sort_field_ < sort_field) {
				rank[i] += node->level_[i].span;
				node = node->level_[i].forward;
			}
			update[i] = node;
		}

		node = node->level_[0].forward;

		//如果key已存在
		if (node != footer_ && node->sort_field_ == sort_field) {
			return false;
		}

		//创建新节点
		Node<SortField, Value>* newNode = CreateNode(sort_field, value);

		//每次最多增加一层
		if (newNode->node_level_ > max_level_) {
			max_level_ = newNode->node_level_;
			rank[max_level_ - 1] = 0;
			update[max_level_ - 1] = header_;
			update[max_level_ - 1]->level_[max_level_ - 1].span = size();
		}

		//调整forward指针
		for (int i = 0; i < newNode->node_level_ ; ++i) {
			node = update[i];
			newNode->level_[i].forward = node->level_[i].forward;
			node->level_[i].forward = newNode;

			newNode->level_[i].span = node->level_[i].span - (rank[0] - rank[i]);
			node->level_[i].span = rank[0] - rank[i] + 1;
		}

		for (int i = max_level_ - 1; i >= newNode->node_level_; --i) {
			update[i]->level_[i].span++;
		}

		++node_count_;
		return true;
	};

	template<typename SortField, typename Value>
	void SkipList<SortField, Value>::DumpAllNodes() const {
		for (const Node<SortField, Value>* itr = begin(); itr != nullptr; itr = itr->next()) {
			DumpNodeDetail(itr);
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	template<typename SortField, typename Value>
	void SkipList<SortField, Value>::DumpNodeDetail(const Node<SortField, Value>* node) const {
		std::cout << "level:" << node->node_level_
			<< ",sort:" << node->sort_field_
			<< ",value:" << node->value_;
		int node_level = node->node_level_ > max_level_ ? max_level_ : node->node_level_;
		for (int i = 0; i <= node_level - 1; ++i) {
			std::cout << ",[forward:" << i 
				<< ",span:" << node->level_[i].span
				<< "]->" << node->level_[i].forward->sort_field_;
		}
		std::cout << " ";
	}

	template<typename SortField, typename Value>
	bool SkipList<SortField, Value>::remove(const SortField& sort_field) {
		//保存的是要删除的前一个节点
		Node<SortField, Value>* update[MAX_LEVEL];
		Node<SortField, Value>* node = header_;
		for (int i = max_level_ - 1; i >= 0; --i) {
			while (node->level_[i].forward != footer_ && node->level_[i].forward->sort_field_ < sort_field) {
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
				update[i]->level_[i].span--;
			} else {
				update[i]->level_[i].forward = node->level_[i].forward;
				update[i]->level_[i].span += (node->level_[i].span - 1);
			}
		}
		delete node;

		//更新max_level_的值，因为有可能在移除一个节点之后，max_level_值会发生变化，及时降低可提高性能
		while (max_level_ > 0 && header_->level_[max_level_ - 1].forward == footer_) {
			--max_level_;
		}

		--node_count_;
		return true;
	};
}
#endif //SKIP_LIST_H_
