#pragma once
#include "Task.h"
#include "LinkedList.h"
#include "Node.h"
#include <vector>


namespace scheduler {

	class node;

	class cal_Day {
	private:
		int year; //연도
		int month;  //월
		int day;   //일
		node* Task_headptr;

	public:
		void insert_Task(int tasknum, Task* task);
		std::vector<Task*> get_Tasks();
		void freeTask();
		cal_Day(int year, int month, int day);
		void freeTaskNum(int tasknum);
	};
}