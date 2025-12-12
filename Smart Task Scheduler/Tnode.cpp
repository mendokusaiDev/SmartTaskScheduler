#include "Tnode.h"
#include "Task.h"

namespace scheduler {

	Tnode::Tnode(Task* task, int task_num)
		: node<Task*>(task){

		this->task_num = task_num;
	}

	Tnode* Tnode::getLink() {
		return (Tnode* )this->next;
	}

	void Tnode::setLink(Tnode* t) {
		this->next = t;
		return;
	}
}