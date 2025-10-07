#include "Node.h"
#include "Calender.h"
#include <iostream>

using namespace std;

namespace scheduler {

	void node::setLink(node* next) {
		this->next = next;
	}

	node* node::getLink() {
		return this->next;
	}

	void node::setData(Task* data) {
		this->task = data;
	}

	Task* node::getData() {
		return this->task;
	}

	node::node(Task* task, int task_num) {
		this->task = task;
		this->task_num = task_num;
	}
}