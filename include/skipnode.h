#pragma once

namespace skiplist {
	template<typename SortField, typename Value>
	class Node;

	template<typename SortField, typename Value>
	struct Level {
		Level() : forward(nullptr), span(0) {};
		Node<SortField, Value>* forward;
		int span;	//跨度
	};

	template<typename SortField, typename Value>
	class Node {
	public:
		Node(int node_level, const SortField& sort_field, const Value& value);
		~Node();
		const Node<SortField, Value>* next() const;

	public:
		const int node_level_; //从1开始
		const SortField sort_field_;
		const Value value_;
		Level<SortField, Value>* level_ = nullptr;
	};

	template<typename SortField, typename Value>
	Node<SortField, Value>::Node(int node_level, const SortField& sort_field, const Value& value)
		: node_level_(node_level), sort_field_(sort_field), value_(value) {
		if (node_level > 0)
			level_ = new Level<SortField, Value>[node_level];
		else
			level_ = nullptr;
	}

	template<typename SortField, typename Value>
	Node<SortField, Value>::~Node() {
		if (level_ != nullptr)
			delete[] level_;
	}

	template<typename SortField, typename Value>
	const Node<SortField, Value>* Node<SortField, Value>::next() const {
		auto node = level_[0].forward;
		//尾结点是整个链表中唯一一个高度为0的节点
		if (node->node_level_ == 0)
			return nullptr;
		return node;
	}
}
