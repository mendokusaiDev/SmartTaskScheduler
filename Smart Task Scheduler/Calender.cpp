#include "Calender.h"
#include "LinkedList.h"
#include "Node.h"
#include "Scheduler.h"
#include <iostream>
#include <chrono>

using namespace std;

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

	void Task::getTime(long long& starttime, long long& endtime) {
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

	void Task::setTime(long long starttime, long long endtime) {
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
		LinkedList::insert_tail(this->Task_headptr, task);
		return;
	}

	vector<Task*> cal_Day::get_Tasks() {

		vector<Task*> ret = LinkedList::get_list(this->Task_headptr);
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

	//////////////////////////////////
	//Calender관련

	//private:

	void Calender::get_current_time(int& year, int& month, int& day, int& hour, int& minute) {
		std::time_t now_utc = std::time(nullptr);
		std::time_t kst_epoch = now_utc + 9 * 60 * 60; // UTC+9

		// gmtime은 static tm*를 반환하므로 즉시 복사해서 사용
		std::tm tm = *std::gmtime(&kst_epoch);

		year = tm.tm_year + 1900;
		month = tm.tm_mon + 1;   // 1..12
		day = tm.tm_mday;      // 1..31
		hour = tm.tm_hour;      // 0..23
		minute = tm.tm_min;       // 0..59
	}

	void Calender::get_first_day_of_week(int &year, int &month, int &day) {  //chatgpt 활용
		tm tm{};
		tm.tm_year = year - 1900;
		tm.tm_mon = month - 1;   // 0..11
		tm.tm_mday = day;
		tm.tm_hour = 12;      // DST/타임존 영향 최소화 (한낮으로 설정)

		// 날짜 정규화 + 요일 채우기 (tm_wday: 0=Sun..6=Sat)
		if (std::mktime(&tm) == -1) return;

		int wday = tm.tm_wday;                       // 0..6
		int offset = (7 + wday - 1) % 7;      // 되돌아갈 일수

		tm.tm_mday -= offset;                          // 주 시작으로 이동
		if (std::mktime(&tm) == -1) return;

		year = tm.tm_year + 1900;
		month = tm.tm_mon + 1;
		day = tm.tm_mday;
		return;
	}

	int max_day_of_month[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

	int Calender::get_max_day(int year, int month) {
		if (month != 2) return max_day_of_month[month];
		if (year % 100 == 0 && year % 400 != 0) return 28;
		if (year % 4 == 0) return 29;
		return 28;
	}

	void Calender::get_next_day(int& year, int& month, int& day) {
		day++;
		if (day <= get_max_day(year, month)) return;
		day = 1; month++;
		if (month <= 12) return;
		month = 1;
		year++;
		return;
	}

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
		int year, month, day, hour, minute;
		get_current_time(year, month, day, hour, minute);
		long long curtime = minute + hour * 100 + day * 10000 + month * 1000000 + year * 100000000;
		int curtime_2 = day + month * 100 + year * 10000;
		vector<Task*> newq, newf;
		//1. 먼저 queue에 있는 작업 완료 표시, 완료 안된것은 newq에 push
		for (int cur : queued) {
			long long starttime, endtime;
			allTasks[cur]->getTime(starttime, endtime);
			if (endtime < curtime) {
				markFinished(cur);
				continue;
			}
			newq.push_back(allTasks[cur]);
		}

		//2. failed에 있는 것 중 enddate가 안지난것만 남기기
		for (int cur : failed) {
			int enddate = allTasks[cur]->getEndDate();
			if (enddate > curtime_2) continue;
			newq.push_back(allTasks[cur]);
		}

		remakeCal(newq, newf);

		return;
	}


	void Calender::remakeCal(vector<Task*>& newq, vector<Task*>& newf) {
		//0. 스케줄 다시 만들기
		S->makeSchedule(newq, newf);
		
		//1. 이전에 할당한 작업 해제  (최적화 필요)
		for (int cur : queued) {
			long long starttime, endtime;
			allTasks[cur]->getTime(starttime, endtime);

			cal_Day* curday = find(starttime / 10000);
			if (curday == nullptr) continue;
			curday->freeTaskNum(cur);
		}

		//2. 새로운 작업 할당
		for (Task* newtask : newq) {
			int tasknum = newtask->getTaskNum();
			long long starttime, endtime;
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

		//S->makeSchedule(tempq, tempf);

		remakeCal(tempq, tempf);

		refreshCal();

		return true;
	}

	bool Calender::deleteTask(int taskNum) {   //미완료 일정에 대해서 삭제 진행
		refreshCal();
		//1. 이미 끝난 일
		if (allTasks[taskNum]->isfinished()) {
			long long starttime, endtime;
			allTasks[taskNum]->getTime(starttime, endtime);
			int date = starttime / 10000;
			cal_Day* cur = find(date);
			cur->freeTaskNum(taskNum);
			allTasks.erase(taskNum);
			return true;
		}

		//2. 아직 안끝난 일
		else {
			vector<Task*> newq, newf;
			for (int cur : queued) {
				if (cur != taskNum) newq.push_back(allTasks[cur]);
			}
			for (int cur : failed) {
				if (cur != taskNum) newq.push_back(allTasks[cur]);
			}

			remakeCal(newq, newf);
			return true;
		}

		return false;
	}

	bool Calender::editTask(int taskNum, string name, int dur, int duedate, int type) {
		if (allTasks.find(taskNum) == allTasks.end()) {
			cout << "edit Task: no task \n";    //디버깅 코드
			return false;
		}

		Task* cur = allTasks[taskNum];
		stat.tasktypes[cur->getType()]--;
		cur->changeTask(name, duedate, dur, type);
		stat.tasktypes[type]++;
		vector<Task*> newq, newf;
		for (int cur : queued) newq.push_back(allTasks[cur]);
		for (int cur : failed) newq.push_back(allTasks[cur]);

		remakeCal(newq, newf);

		return true;

	}

	void Calender::markFinished(int taskNum) {  //일단 마킹만 시행
		allTasks[taskNum]->done();
		return;
	}


	bool Calender::get_Day(vector<Task*> & tasks, int year, int month, int day) {
		int date = day + month * 100 + year * 10000;
		
		cal_Day* cur = find(date);
		if (cur == nullptr) return false;

		tasks = cur->get_Tasks();
		return true;

	}

	bool Calender::get_Week(vector<Task*> & tasks, int year, int month, int day) {   //현재 날짜 기준으로 첫 주의 날짜 (월~일)
		get_first_day_of_week(year, month, day);

		for (int i = 0; i < 7; i++) {
			vector<Task*> temp;
			get_Day(temp, year, month, day);
			for (Task* t : temp) tasks.push_back(t);
			get_next_day(year, month, day);
		}

		return true;
	}

	bool Calender::get_Month(vector<Task*>  & tasks, int year, int month, int day) {

		if (year_headptr.find(year) == year_headptr.end()) return false;

		cal_Year* curY = year_headptr[year];
		if (curY == nullptr) return false;
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