#pragma once
#include "Priority_queue.h"
#include <vector>
#include "Task.h"

using namespace std;

namespace scheduler {
	
	class scheduler {
		/*
			멤버 변수: 우선순위 큐
			멤버 함수: 앞으로 일정 만들기
					   스케줄러 인터벌 설정
		*/

	public:
		vector<Task *> makeSchedule(vector<Task *> queued);
		void changeInterval(int interval);

	private:
		PriorityQueue* pq;
		int interval;

	};
}