#pragma once
#include "Node.h"
#include "LinkedList.h"
#include "Task.h"
#include <type_traits>



namespace scheduler {
	
	class LinkedList;

	template<class Item>
	class Set {
	private:
		node<Item> * headptr;

	public:
		Item get_Item(int n) {
			node<Item>* cur = headptr;

			if (cur == NULL) return NULL;
			while (cur != NULL) {
				if (*(cur->getData()) == n) return cur->getData();

				cur = cur->getLink();
			}

			return NULL;
		}

		void put_Item(Item t) {
			if (CheckForOccurence(t)) return;
			LinkedList::insert_head(headptr, t);
		}



		bool CheckForOccurence(Item t) {

			node<Item>* cur = headptr;
			if (cur == NULL) return false;

			if (std::is_pointer<Item>::value) {

				while (cur != NULL) {
					if (*cur==t) return true;

					cur = cur->getLink();
				}

				return false;
			}
			return false;
		}
	
		Set() {
			headptr = NULL;
		}
	};
}