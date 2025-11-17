#include "Calender.h"
#include "saveFile.h"
#include "cal_Day.h"
#include "cal_Month.h"
#include "cal_Year.h"
#include "Task.h"
#include "Scheduler.h"

#include <fstream>

namespace scheduler {

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
		int curYear = date / 10000;
		int curMonth = (date % 10000) / 100;
		int curDay = date % 100;
		cal_Year* tempY; cal_Month* tempM;
		if ((tempY = year_headptr.get_Item(curYear))==nullptr) return nullptr;

		if ((tempM = tempY->get_Month(curMonth)) == nullptr) return nullptr;

		return tempM->get_Day(curDay);
	}

	cal_Day* Calender::newDay(int date) {   //20251010 형식의 날을 만들고 반환.
		int curYear = date / 10000;
		int curMonth = (date % 10000) / 100;
		int curDay = date % 100;

		cal_Year* newY; cal_Month* newM;
		if (year_headptr.get_Item(curYear) == nullptr) {
			year_headptr.put_Item(new cal_Year(curYear));
		}
		newY = year_headptr.get_Item(curYear);

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
		long long curtime = minute + hour * 100 + day * 10000 + month * 1000000LL + year * 100000000LL;
		int curtime_2 = day + month * 100 + year * 10000;
		std::vector<Task*> newq, newf;
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


	void Calender::remakeCal(std::vector<Task*>& newq, std::vector<Task*>& newf) {
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
				std::cout << "job number " << tasknum << " starttime not set\n";
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
		this->S = new Scheduler(15); //스케줄러 생성
		restore();
		refreshCal();
	}

	Calender::~Calender() {
		save();
	}

	bool Calender::addTask(std::string name, long long dur, long long duedate, int type) {
		refreshCal();   //먼저 시간차에 따른 캘린더 갱신
		/*
			1. task 만들기
			2. refresh cal
		
		*/
		Task * t = new Task(name, duedate, dur, type, tasks_count);
		this->allTasks[this->tasks_count] = t;
		this->tasks_count++;

		std::vector<Task*> tempq, tempf;
		for (int ptr : queued) tempq.push_back(this->allTasks[ptr]);
		for (int ptr : failed) tempq.push_back(this->allTasks[ptr]);
		tempq.push_back(t);
		//S->makeSchedule(tempq, tempf);

		remakeCal(tempq, tempf);

		refreshCal();

		return true;
	}

	bool Calender::addFixedTask(std::string name, long long startime, long long endtime, int type) {
		refreshCal();

		Task* t = new Task(1, name, startime, endtime, type, tasks_count);  //고정 일정
		this->allTasks[this->tasks_count] = t;
		this->tasks_count++;

		std::vector<Task*> tempq, tempf;
		for (int ptr : queued) tempq.push_back(this->allTasks[ptr]);
		for (int ptr : failed) tempq.push_back(this->allTasks[ptr]);
		tempq.push_back(t);
		//S->makeSchedule(tempq, tempf);

		remakeCal(tempq, tempf);

		refreshCal();

		return true;
	}
	void Calender::setUninterruptedTime(long long start, long long end) {
		S->setUnterruptedTime(start, end);
	}

	bool Calender::deleteTask(int taskNum) {   //미완료 일정에 대해서 삭제 진행
		refreshCal();
		//1. 이미 끝난 일
		if (allTasks.find(taskNum) == allTasks.end()) {
			return false;
		}
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
			std::vector<Task*> newq, newf;
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

	bool Calender::editTask(int taskNum, std::string name, int dur, int duedate, int type) {
		if (allTasks.find(taskNum) == allTasks.end()) {
			std::cout << "edit Task: no task \n";    //디버깅 코드
			return false;
		}

		Task* cur = allTasks[taskNum];
		stat.tasktypes[cur->getType()]--;
		cur->changeTask(name, duedate, dur, type);
		stat.tasktypes[type]++;
		std::vector<Task*> newq, newf;
		for (int cur : queued) newq.push_back(allTasks[cur]);
		for (int cur : failed) newq.push_back(allTasks[cur]);

		remakeCal(newq, newf);

		return true;

	}

	void Calender::markFinished(int taskNum) {  //일단 마킹만 시행
		allTasks[taskNum]->done();
		return;
	}


	bool Calender::get_Day(std::vector<Task*> & tasks, int year, int month, int day) {
		int date = day + month * 100 + year * 10000;
		
		cal_Day* cur = find(date);
		if (cur == nullptr) return false;

		tasks = cur->get_Tasks();
		return true;

	}

	bool Calender::get_Week(std::vector<Task*> & tasks, int year, int month, int day) {   //현재 날짜 기준으로 첫 주의 날짜 (월~일)
		get_first_day_of_week(year, month, day);

		for (int i = 0; i < 7; i++) {
			std::vector<Task*> temp;
			get_Day(temp, year, month, day);
			for (Task* t : temp) tasks.push_back(t);
			get_next_day(year, month, day);
		}

		return true;
	}

	bool Calender::get_Month(std::vector<Task*>  & tasks, int year, int month, int day) {

		if (year_headptr.get_Item(year)==nullptr) return false;
		cal_Year* curY = year_headptr.get_Item(year);

		if (curY == nullptr) return false;

		cal_Month* curM = curY->get_Month(month);
		if (curM == nullptr) return false;
		cal_Day* curD = nullptr;

		for (int i = 1; i <= 31; i++) {   //모든 날 순회하기
			curD = curM->get_Day(i);
			if (curD == nullptr) continue;
			std::vector<Task*> tempv = curD->get_Tasks();
			for (Task* t : tempv) tasks.push_back(t);
		}

		return true;
	}


	Calender::calStats Calender::getStatistics() {

		return stat;
	}

	bool Calender::changeInterval(int interval, std::vector<Task*>  & queued, std::vector<Task*> & failed) {
		S->ChangeInterval(interval);
		
		std::vector<Task*> tempq, tempf;
		for (int ptr : this->queued) tempq.push_back(this->allTasks[ptr]);
		for (int ptr : this->failed) tempq.push_back(this->allTasks[ptr]);

		S->makeSchedule(tempq, tempf);

		remakeCal(tempq, tempf);

		refreshCal();

		queued = tempq;
		failed = tempf;

		return true;

	}

	void Calender::save() {
		std::ofstream ofs("./save.tmp", std::ios::binary | std::ios::trunc);
		if (!ofs) {
			std::cout << "error opening file" << std::endl;

			return;
		}

		int taskSize = allTasks.size(); //task 개수 저장하기
		ofs.write((char*)&taskSize, sizeof(int));
		//std::string name;
		//long long enddate=0, duration=0, starttime=0, endtime=0;
		//int type=0, tasknum=0;
		//bool finished=0;
		for (auto &cur : allTasks) {
			saveFile* t = new saveFile(cur.second);
			std::string str = cur.second->getTaskName();
			int len = str.length();
			int sFsize = sizeof(saveFile);

			ofs.write((char*)&len, sizeof(int));
			ofs.write((char*)str.c_str(), len);
			ofs.write((char*)&sFsize, sizeof(int));
			ofs.write((char*)t, sFsize);
			delete t;
		}

		int queuedSize = queued.size();  //queued 저장
		ofs.write((char*)&queuedSize, sizeof(int));
		for (auto cur : queued) {
			ofs.write((char*)&cur, sizeof(int));
		}

		int failedSize = failed.size();   //failed 저장
		ofs.write((char*)&failedSize, sizeof(int));
		for (auto cur : failed) {
			ofs.write((char*)&cur, sizeof(int));
		}

		int finishedCount = finished.size();   //finished 저장
		ofs.write((char*)&finishedCount, sizeof(int));
		for (auto cur : finished) {
			ofs.write((char*)&cur, sizeof(int));
		}

		ofs.write((char*)&stat, sizeof(calStats));   //캘린더 stat 저장
		ofs.write((char*)&tasks_count, sizeof(int)); //캘린더 tasks_count 저장
		
		long long S_int = S->get_interval();
		ofs.write((char*)&S_int, sizeof(long long)); //interval 저장하기
		//std::cout << "interval: " << S_int << std::endl;

		ofs.close();

		return;
	}

	void Calender::restore() {
		std::ifstream ifs("./save.tmp", std::ios::binary);
		if (!ifs) return;

		std::cout << "restoring" << std::endl;

		//task 개수 가져오기
		int taskCount = 0;
		ifs.read((char*)&taskCount, sizeof(int));
		Task* cur= nullptr;
		for (int i = 0; i < taskCount; i++) {  //allTasks 저장하기
			int slen = 0, sflen = 0;
			std::string str; saveFile* fs = new saveFile();
			ifs.read((char*)&slen, sizeof(int));
			str.resize(slen);
			ifs.read((char*)str.c_str(), slen);
			ifs.read((char*)&sflen, sizeof(int));
			ifs.read((char*)fs, sflen);
			cur = new Task(str, fs);
			allTasks[cur->getTaskNum()] = cur;
		}

		//queued 저장하기
		int queuedCount = 0;
		ifs.read((char*)&queuedCount, sizeof(int));
		int queuedN = -1;
		for (int i = 0; i < queuedCount; i++) {
			ifs.read((char*)&queuedN, sizeof(int));
			queued.push_back(queuedN);
		}

		//failed 저장하기
		int failedCount = 0;
		ifs.read((char*)&failedCount, sizeof(int));
		int failedN = -1;
		for (int i = 0; i < failedCount; i++) {
			ifs.read((char*)&failedN, sizeof(int));
			queued.push_back(failedN);
		}

		//finished 저장하기
		int finCount = 0;
		ifs.read((char*)&finCount, sizeof(int));
		int finN = -1;
		for (int i = 0; i < finCount; i++) {
			ifs.read((char*)&finN, sizeof(int));
			queued.push_back(finN);
		}

		ifs.read((char*)&stat, sizeof(calStats));  //통계 저장하기
		ifs.read((char*)&tasks_count, sizeof(int)); //캘린더 tasks_count 저장

		long long S_int = -1;
		ifs.read((char*)&S_int, sizeof(long long));   //스케줄러 인터벌 저장
		//std::cout << "interval: " << S_int << std::endl;
		S->ChangeInterval(S_int);

		ifs.close();

		return;
	}
}