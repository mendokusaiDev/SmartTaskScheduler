#pragma once
#include "Calender.h"
#include "Node.h"


namespace scheduler {

	class LinkedList {
	public:
		static void insert(node* headptr, node* ins);
		static vector<node*> get_list(node* headptr);
		static void free_List(node*& headptr);
	};
}