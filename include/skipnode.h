#ifndef SKIP_LIST_NODE_H_
#define SKIP_LIST_NODE_H_

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
		Node(int node_level, const SortField& sort_field, const Value& value) 
			: node_level_(node_level), sort_field_(sort_field), value_(value) {
			if (node_level_ > 0) {
				level_ = new Level<SortField, Value>[node_level_];
			}
			else {
				level_ = nullptr;
			}
		};

		~Node() {
			if (level_ != nullptr)
				delete[] level_;
		};

		const Node<SortField, Value>* next() const {
			auto node = level_[0].forward;
			//尾结点是整个链表中唯一一个高度为0的节点
			if (node->node_level_ == 0)
				return nullptr;
			return node;
		};

	public:
		const SortField sort_field_;
		const Value value_;
		const int node_level_; //从1开始
		Level<SortField, Value>* level_;
	};
}

#endif //SKIP_LIST_NODE_H_
