#pragma once
#include "Calender.h"
#include "Node.h"
namespace scheduler {

	static class LinkedList {
	public:
		static void insert(node* headptr, node* ins);
		vector<node*> get_list(node* headptr);
		void free_List();
	};
}