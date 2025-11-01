#pragma once
#include <vector>
#include "Node.h"


namespace scheduler {

	class node;
	
	class LinkedList {
	public:
		static void insert_tail(node*& headptr, Task * task);
		static void insert_head(node*& headptr, Task* task);
		static std::vector<Task*> get_list(node* headptr);
		static void free_List(node*& headptr);
		static void deleteTask(node *& headptr, int tasknum);
		static void deleteHeadTask(node*& headptr);
	};
}