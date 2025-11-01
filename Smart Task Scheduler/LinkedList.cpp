#include "LinkedList.h"


namespace scheduler {

	void LinkedList::insert_tail(node*& headptr, Task* task) {
		if (headptr == nullptr) {
			insert_head(headptr, task);
			return;
		}

		node* ins = new node(task, task->getTaskNum());
		node* cur = headptr;
		while (cur->getLink() != nullptr) {
			cur = cur->getLink();
		}

		cur->setLink(ins);

		return;
	}


	void LinkedList::insert_head(node*& headptr, Task* task) {
		headptr = new node(task, task->getTaskNum());

		return;
	}


	std::vector<Task*> LinkedList::get_list(node* headptr) {
		node* cur = headptr;
		std::vector<Task*> ret;

		while (cur != nullptr) {
			ret.push_back(cur->getData());
			cur = cur->getLink();
		}

		return ret;
	}

	void LinkedList::free_List(node*& headptr) {
		node* cur = headptr;

		while (cur != nullptr) {
			node* next = cur->getLink();
			delete cur;
			cur = next;
		}

		headptr = nullptr;

		return;
	}

	void LinkedList::deleteTask(node*& headptr, int tasknum) {
		node* cur = headptr;
		node* prev = headptr;

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

	void LinkedList::deleteHeadTask(node*& headptr) {
		node* del = headptr;
		headptr = del->getLink();

		delete del;

		return;
	}

}