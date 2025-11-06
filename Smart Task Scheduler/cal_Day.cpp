#include "cal_Day.h"
#include "LinkedList.h"
#include "Tnode.h"
#include "Task.h"

namespace scheduler {
	///////////////////////////////////////////////
	//cal Day °ü·Ã

	bool cal_Day::operator==(int n) {
		if (this->day == n) return true;
		return false;
	}

	bool cal_Day::operator==(cal_Day d) const {
		if (this->day == d.day) return true;
		return false;
	}


	void cal_Day::insert_Task(int tasknum, Task* task) {
		LinkedList::insert_tail(this->Task_headptr, task);
		return;
	}

	std::vector<Task*> cal_Day::get_Tasks() {

		std::vector<Task*> ret = LinkedList::get_list(this->Task_headptr);
		return ret;
	}

	void cal_Day::freeTask() {
		LinkedList::free_List(this->Task_headptr);
		return;
	}

	void cal_Day::freeTaskNum(int tasknum) {
		LinkedList::deleteTask(Task_headptr, tasknum);
	}

	cal_Day::cal_Day(int year, int month, int day) {   //cal_Day constructor
		this->year = year;
		this->month = month;
		this->day = day;
		this->Task_headptr = nullptr;
	}
}