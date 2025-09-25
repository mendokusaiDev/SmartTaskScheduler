#pragma once
#include "Task.h"

namespace scheduler {
	class PriorityQueue {
		
		/*
		
			멤버변수: Task (또는 Task 객체의 주소) 를 저장하는 배열 또는 힙 구조
					  
			멤버함수: Task 넣기
			          Task 빼기
					  Task 보기
					  Queue clear 하기
		
		*/

	private:


	public:
		void push(Task* t);
		void pop();
		Task* front();
		void clear();
		PriorityQueue();
		~PriorityQueue();
	};
}