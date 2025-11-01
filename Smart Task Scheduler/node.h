#pragma once
#include "Task.h"
#include "Calender.h"


namespace scheduler {

	class node {
	private:
		Task* task;
		node* next;
		int task_num;
	public:
		void setLink(node* next);
		node* getLink();
		void setData(Task* data);
		Task* getData();
		int get_tasknum() { return task_num; }
		node(Task * task, int task_num);
	};
}