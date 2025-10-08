#include "Calender.h"
#include "LinkedList.h"
#include "Node.h"
#include "Scheduler.h"
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

	void Task::getTime(int& starttime, int& endtime) {
		starttime = this->starttime;
		endtime = this->endtime;
		return;
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

	void Task::setTime(int startime, int endtime) {
		this->starttime = starttime;
		this->endtime = endtime;
	}

	void Task::resetTime() {
		setTime(-1, -1);
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
		LinkedList::insert(this->Task_headptr, task);
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

	void cal_Day::freeTaskNum(int tasknum) {
		LinkedList::deleteTask(Task_headptr, tasknum);
	}

	cal_Day::cal_Day(int year, int month, int day) {   //cal_Day constructor
		this->year = year;
		this->month = month;
		this->day = day;
		this->Task_headptr = nullptr;
	}


	//Calender관련

	//private:

	cal_Day* Calender::find(int date) {   //20251010 형식의 날을 찾기
		int curYear = date / 1000000;
		int curMonth = (date % 10000) / 100;
		int curDay = date % 100;
		cal_Year* tempY; cal_Month* tempM;
		if (year_headptr.find(curYear) == year_headptr.end()) return nullptr;
		tempY = year_headptr[curYear];

		if ((tempM = tempY->get_Month(curMonth)) == nullptr) return nullptr;

		return tempM->get_Day(curDay);
	}

	cal_Day* Calender::newDay(int date) {   //20251010 형식의 날을 만들고 반환.
		int curYear = date / 1000000;
		int curMonth = (date % 10000) / 100;
		int curDay = date % 100;

		cal_Year* newY; cal_Month* newM;
		if (year_headptr.find(curYear) == year_headptr.end()) {
			year_headptr[curYear] = new cal_Year(curYear);
		}
		newY = year_headptr[curYear];

		if (newY->get_Month(curMonth) == nullptr) {
			newY->set_Month(curMonth);
		}

		newM = newY->get_Month(curMonth);

		if (newM->get_Day(curDay) == nullptr)
			newM->set_Day(curDay);

		return newM->get_Day(curDay);
	}

	void Calender::refreshCal() {   //시간에 따른 calender 갱신



	}


	void Calender::remakeCal(vector<Task*>& newq, vector<Task*>& newf) {
		//1. 이전에 할당한 작업 해제  (최적화 필요)
		for (int cur : queued) {
			int starttime, endtime;
			allTasks[cur]->getTime(starttime, endtime);

			cal_Day* curday = find(starttime / 10000);
			if (curday == nullptr) continue;
			curday->freeTaskNum(cur);
		}

		//2. 새로운 작업 할당
		for (Task* newtask : newq) {
			int tasknum = newtask->getTaskNum();
			int starttime, endtime;
			newtask->getTime(starttime, endtime);

			if (starttime == -1) {
				cout << "job number " << tasknum << " starttime not set\n";
				continue;
			}

			cal_Day* tempD = newDay(starttime / 10000);
			tempD->insert_Task(tasknum, newtask);
		}

		//3. Calender queue, failed 갱신
		queued.clear(); failed.clear();
		for (Task* cur : newq) queued.push_back(cur->getTaskNum());
		for (Task* cur : newf) failed.push_back(cur->getTaskNum());


		return;
	}


	//public:

	Calender::Calender() {
		this->S = new Scheduler(); //스케줄러 생성
	}

	bool Calender::addTask(string name, int dur, int duedate, int type) {
		refreshCal();   //먼저 시간차에 따른 캘린더 갱신
		/*
			1. task 만들기
			2. refresh cal
		
		*/

		this->allTasks[this->tasks_count] = new Task(name, duedate, dur, type, tasks_count);
		this->tasks_count++;

		vector<Task*> tempq, tempf;
		for (int ptr : queued) tempq.push_back(this->allTasks[ptr]);
		for (int ptr : failed) tempq.push_back(this->allTasks[ptr]);

		S->makeSchedule(tempq, tempf);

		remakeCal(tempq, tempf);

		refreshCal();

		return true;
	}

	bool Calender::deleteTask(int taskNum) {   //미완료 일정에 대해서 삭제 진행
		refreshCal();

		//1. 이미 지난 일


	}

	bool Calender::editTask(int taskNum, string name, int dur, int duedate, int type) {



	}

	void Calender::markFinished(int taskNum) {




	}


	bool Calender::get_Day(vector<Task*> & tasks, int year, int month, int day) {
		int date = day + month * 100 + year * 10000;
		
		cal_Day* cur = find(date);
		if (cur == nullptr) return false;

		tasks = cur->get_Tasks();
		return true;

	}

	bool Calender::get_Week(vector<Task*> & tasks, int year, int month, int day) {   //생각해봐야됨.




	}

	bool Calender::get_Month(vector<Task*>  & tasks, int year, int month, int day) {

		if (year_headptr.find(year) == year_headptr.end()) return false;

		cal_Year* curY = year_headptr[year];
		cal_Month* curM = curY->get_Month(month);
		if (curM == nullptr) return false;
		cal_Day* curD = nullptr;

		for (int i = 1; i <= 31; i++) {   //모든 날 순회하기
			curD = curM->get_Day(i);
			if (curD == nullptr) continue;
			vector<Task*> tempv = curD->get_Tasks();
			for (Task* t : tempv) tasks.push_back(t);
		}

		return true;
	}


	Calender::calStats Calender::getStatistics() {

		return stat;
	}

	bool Calender::changeInterval(int interval, vector<Task*>  & queued, vector<Task*> & failed) {
		S->ChangeInterval(interval);
		
		vector<Task*> tempq, tempf;
		for (int ptr : this->queued) tempq.push_back(this->allTasks[ptr]);
		for (int ptr : this->failed) tempq.push_back(this->allTasks[ptr]);

		S->makeSchedule(tempq, tempf);

		remakeCal(tempq, tempf);

		refreshCal();

		queued = tempq;
		failed = tempf;

		return true;

	}


	
}