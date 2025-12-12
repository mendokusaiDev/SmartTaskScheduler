#include "Node.h"

namespace scheduler {

	template<class Item>
	void node<Item>::setLink(node<Item>* next) {
		this->next = next;
	}

	template<class Item>
	node<Item>* node<Item>::getLink() {
		return this->next;
	}

	template<class Item>
	void node<Item>::setData(Item data) {
		this->data = data;
	}

	template<class Item>
	Item node<Item>::getData() {
		return this->data;
	}

	template<class Item>
	node<Item>::node(Item task) {
		this->data = task;
		//this->task_num = task_num;
		this->next = nullptr;
	}
}