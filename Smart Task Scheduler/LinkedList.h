#pragma once
#include <vector>


namespace scheduler {

	template<class Item>
	class node;
	class Tnode;
	class Task;
	
	class LinkedList {
	public:
		template<class Item>
		static void insert_tail(node<Item>*& headptr, Item task) {
			if (headptr == nullptr) {
				insert_head(headptr, task);
				return;
			}

			node<Item>* ins = new node(task);
			node<Item>* cur = headptr;
			while (cur->getLink() != nullptr) {
				cur = cur->getLink();
			}

			cur->setLink(ins);

			return;
		}

		static void insert_tail(Tnode*& headtpr, Task* task);

		template<class Item>
		static void insert_head(node<Item>*& headptr, Item task) {
			node<Item>* ins = new node<Item>(task);
			
			if(headptr==nullptr)
				headptr = ins;
			else {
				ins->setLink(headptr);
				headptr = ins;
			}

			return;
		}

		static void insert_head(Tnode*& headptr, Task* task);

		template<class Item>
		static std::vector<Item> get_list(node<Item>* headptr) {
			node<Item>* cur = headptr;
			std::vector<Item> ret;

			while (cur != nullptr) {
				ret.push_back(cur->getData());
				cur = cur->getLink();
			}

			return ret;
		}

		template<class Item>
		static void free_List(node<Item>*& headptr) {
			node<Item>* cur = headptr;

			while (cur != nullptr) {
				node<Item>* next = cur->getLink();
				delete cur;
				cur = next;
			}

			headptr = nullptr;

			return;
		}

		static void free_List(Tnode*& headptr);

		//template<class Item>
		static void deleteTask(Tnode*& headptr, int tasknum);

		//template<class Item>
		static void deleteHeadTask(Tnode*& headptr);
	};
}