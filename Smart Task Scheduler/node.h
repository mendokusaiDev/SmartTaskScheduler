#pragma once
#include "Calender.h"
namespace scheduler {

	class node {
	private:
		Task* task;
	public:
		void setLink(node* next);
		node* getLink();
		void setData(Task* data);
		Task* getData();
	};
}