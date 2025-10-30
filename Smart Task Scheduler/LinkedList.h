#pragma once
#include "Calender.h"
#include "Node.h"


namespace scheduler {

	class LinkedList {
	public:
		static void insert_tail(node*& headptr, Task * task);
		static vector<Task*> get_list(node* headptr);
		static void free_List(node*& headptr);
		static void deleteTask(node *& headptr, int tasknum);
	};
}