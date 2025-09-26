#pragma once
#include "Calender.h"

namespace scheduler {

	class PriorityQueue {

	private:


	public:
		void clear();
		void push(Task *task);
		void pop();
		Task* front();
	};
}