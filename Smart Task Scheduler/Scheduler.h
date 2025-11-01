#pragma once
#include "Priority_queue.h"
#include <vector>

class scheduler::PriorityQueue;

namespace scheduler {

	class Scheduler {

	private:
		long long interval;
		

	public:
		PriorityQueue* q;
		bool ChangeInterval(long long time);
		void makeSchedule(std::vector<Task*> & original, std::vector<Task*> & failed);
		Scheduler(long long time) { this->interval = time; }
	};
}