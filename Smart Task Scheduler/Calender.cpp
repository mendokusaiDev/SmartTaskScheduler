#include "Calender.h"
#include "LinkedList.h"
#include "Node.h"
#include <iostream>

namespace scheduler {

	//task 관련
	Task::Task(string name, int enddate, int duration, int type, int taskNum) {
		this->name = name;
		this->enddate = enddate;
		this->duration = duration;
		this->type = type;
		this->starttime = -1;
		this->TaskNum = taskNum;
		this->finished = 0;
		this->endtime = -1;
	}


	void Task::changeTask(string name, int enddate, int duration, int type) {
		this->name = name;
		this->enddate = enddate;
		this->duration = duration;
		this->type = type;
	}


	int Task::getTaskNum() {
		return this->TaskNum;
	}



	///////////////////////////////////
	//cal Year 관련
	cal_Month* cal_Year::get_Month(int month){
		if (this->month_headptr.find(month) == this->month_headptr.end())
			return nullptr;

		return this->month_headptr[month];
	}


	void cal_Year::set_Month(int month) {
		
		if (get_Month(month) != nullptr) {
			std::cout << "error: " << month << "on " << this->year << "already exists\n";
			return;
		}

		this->month_headptr[month] = new cal_Month(this->year, month); //수정하기

	}

	cal_Year::cal_Year(int year) {
		this->year = year;
	}


	////////////////////////////////////////////////
	//cal Month 관련
	cal_Day* cal_Month::get_Day(int day) {
		if (this->day_headptr.find(day) == this->day_headptr.end()) {
			return nullptr;
		}

		return this->day_headptr[day];
	}

	void cal_Month::set_Day(int day) {

		if (get_Day(day) != nullptr) {
			std::cout << "error: " << day << "on " << this->month << "already exists\n";
			return;
		}

		this->day_headptr[day] = new cal_Day(this->year, this->month, day);
		return;

	}


	cal_Month::cal_Month(int year, int month) {
		this->year = year;
		this->month = month;
	}


	///////////////////////////////////////////////
	//cal Day 관련

	void cal_Day::insert_Task(int tasknum, Task* task) {
		LinkedList::insert(this->Task_headptr, new node(task, tasknum));
		return;
	}

	vector<Task*> cal_Day::get_Tasks() {
		vector<node*> temp = LinkedList::get_list(this->Task_headptr);
		vector<Task*> ret;
		for (node* ptr : temp) {
			ret.push_back(ptr->getData());
		}

		return ret;
	}

	void cal_Day::freeTask() {
		LinkedList::free_List(this->Task_headptr);
		return;
	}

	cal_Day::cal_Day(int year, int month, int day) {   //cal_Day constructor
		this->year = year;
		this->month = month;
		this->day = day;
		this->Task_headptr = nullptr;
	}


	//Calender관련

	//private:

	void Calender::refreshCal() {



	}


	//public:

	Calender::Calender() {



	}

	bool Calender::addTask(string name, int dur, int duedate, int type) {



	}

	bool Calender::deleteTask(int taskNum) {



	}

	bool Calender::editTask(int taskNum, string name, int dur, int duedate, int type) {



	}

	void Calender::markFinished(int taskNum) {




	}


	bool Calender::get_Day(vector<Task*> tasks, int year, int month, int day) {




	}

	bool Calender::get_Week(vector<Task*> tasks, int year, int month, int day) {




	}

	bool Calender::get_Month(vector<Task*> tasks, int year, int month, int day) {




	}

	Calender::calStats Calender::getStatistics() {




	}

	bool Calender::changeInterval(int interval, vector<Task*> queued, vector<Task*> failed) {





	}


	
}