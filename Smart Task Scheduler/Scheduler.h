#pragma once
#include "Priority_queue.h"

namespace scheduler {

	class Scheduler {

	private:
		int interval;
		

	public:
		PriorityQueue* q;
		bool ChangeInterval(int time);
		void makeSchedule(vector<Task*>& original, vector<Task*>& failed);
	};
}