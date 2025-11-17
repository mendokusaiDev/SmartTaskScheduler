#pragma once
#include "Priority_queue.h"
#include <vector>

class scheduler::PriorityQueue;

namespace scheduler {

	class Scheduler {

	private:
		long long interval;
		long long unInterruptStart = 0;
		long long unInterruptEnd = 0;

	public:
		PriorityQueue* q=nullptr;
		bool ChangeInterval(long long time);
		long long get_interval() { return interval; }
		void setUnterruptedTime(long long start, long long end);
		void makeSchedule(std::vector<Task*> & original, std::vector<Task*> & failed);
		Scheduler(long long time) { this->interval = time; }
	};
}