#include "Set.h"
#include "LinkedList.h"

namespace scheduler {

	template<class Item>
	Item Set<Item>::get_Item(int n) {
		node<Item>* cur = headptr;

		if (cur == NULL) return NULL;
		while (cur != NULL) {
			if (*(cur->getData()) == n) return cur->getData();

			cur = cur->getLink();
		}

		return NULL;
	}

	template<class Item>
	void Set<Item>::put_Item(Item t) {
		if (CheckForOccurence(t)) return;
		LinkedList::insert_head(headptr, t);
	}

	template<class Item>
	bool Set<Item>::CheckForOccurence(int n) {

		node<Item>* cur = headptr;

		if (cur == NULL) return false;
		while (cur != NULL) {
			if (*(cur->getData()) == n) return true;

			cur = cur->getLink();
		}

		return false;
	}

	template<class Item>
	Set<Item>::Set() {
		headptr = NULL;
	}
}