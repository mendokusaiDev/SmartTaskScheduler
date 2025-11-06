#pragma once
#include <vector>


namespace scheduler {

	class Tnode;
	class Task;

	class cal_Day {
	private:
		int year; //¿¬µµ
		int month;  //¿ù
		int day;   //ÀÏ
		Tnode* Task_headptr;

	public:
		void insert_Task(int tasknum, Task* task);
		std::vector<Task*> get_Tasks();
		void freeTask();
		cal_Day(int year, int month, int day);
		void freeTaskNum(int tasknum);
		bool operator==(int n);
		bool operator==(cal_Day d) const;
	};
}