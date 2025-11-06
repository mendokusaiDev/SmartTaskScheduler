#include "LinkedList.h"
#include "Task.h"
#include "Tnode.h"
#include "Node.h"
#include <iostream>

namespace scheduler {


	void LinkedList::insert_tail(Tnode*& headptr, Task* task) {
		if (headptr == nullptr) {
			insert_head(headptr, task);
			return;
		}

		Tnode* ins = new Tnode(task, task->getTaskNum());
		Tnode* cur = headptr;
		while (cur->getLink() != nullptr) {
			cur = cur->getLink();
		}

		cur->setLink(ins);
		
		return;
	}

	void LinkedList::insert_head(Tnode*& headptr, Task* task) {
		headptr = new Tnode(task, task->getTaskNum());

		return;
	}


	//수정필요
	//template<class Item>
	void LinkedList::deleteTask(Tnode*& headptr, int tasknum) {
		Tnode* cur = headptr;
		Tnode* prev = headptr;

		if (headptr == nullptr) {
			std::cout << "error: headptr is null from deleteTask" << std::endl;
			return;
		}

		while (cur != nullptr) {
			if (cur->get_tasknum() == tasknum) break;
			prev = cur;
			cur = cur->getLink();
		}
		if (cur == headptr) {
			LinkedList::deleteHeadTask(headptr);
			return;
		}


		prev->setLink(cur->getLink());
		delete cur;

		return;
	}

	//template<class Item>
	void LinkedList::deleteHeadTask(Tnode*& headptr) {
		Tnode* del = headptr;
		headptr = del->getLink();

		delete del;

		return;
	}

	void LinkedList::free_List(Tnode*& headptr) {
		Tnode* cur = headptr;

		while (cur != nullptr) {
			Tnode* next = cur->getLink();
			delete cur;
			cur = next;
		}

		headptr = nullptr;

		return;
	}

}