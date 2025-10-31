#pragma once
#include "Priority_queue.h"
#include <vector>

class scheduler::PriorityQueue;

namespace scheduler {

	class Scheduler {

	private:
		int interval;
		

	public:
		PriorityQueue* q;
		bool ChangeInterval(int time);
		void makeSchedule(std::vector<Task*> & original, std::vector<Task*> & failed);
	};
}