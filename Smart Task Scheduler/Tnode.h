#pragma once
#include "Node.h"
namespace scheduler {

	template<class Item>
	class node;
	class Task;
	

	//template <class Item>
	class Tnode: public node<Task*>{
	private:
		int task_num;

	public:
		int get_tasknum() { return task_num; }

		Tnode(Task* task, int task_num);
		Tnode* getLink();
		void setLink(Tnode * t);
		//int get_tasknum() { return task_num; }
	};
}