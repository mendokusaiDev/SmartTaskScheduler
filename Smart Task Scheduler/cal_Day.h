#pragma once
#include "Task.h"
#include "LinkedList.h"
#include "Node.h"
#include <vector>


namespace scheduler {

	class node;

	class cal_Day {
	private:
		int year; //¿¬µµ
		int month;  //¿ù
		int day;   //ÀÏ
		node* Task_headptr;

	public:
		void insert_Task(int tasknum, Task* task);
		std::vector<Task*> get_Tasks();
		void freeTask();
		cal_Day(int year, int month, int day);
		void freeTaskNum(int tasknum);
	};
}