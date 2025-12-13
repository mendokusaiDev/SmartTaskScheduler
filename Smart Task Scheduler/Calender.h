#pragma once
#define _CRT_SECURE_NO_WARNINGS
//#include "Scheduler.h"
//#include "Node.h"
//#include "cal_Year.h"
//#include "cal_Day.h"
//#include "Task.h"
#include "Set.h"
#include <map>
#include <vector>
#include <string>

//using namespace std;

namespace scheduler {

	class cal_Year;
	class cal_Day;
	class Task;
	class Scheduler;

	class Calender {
	private:
		std::vector<int> queued; //일정에 할당 된 작업
		std::vector<int> failed;  //일정 할당에 실패한 작업
		std::vector<int> finished;  //끝난 일정
		std::map<int, Task*> allTasks;  //모든 작업
		//cal_Year* headptr;  //연도 headptr
		Set<cal_Year*> year_headptr; //연도 headptr
		int tasks_count = 0;  //누적 일정 개수 (이게 일정의 고유번호가 됨)
		Scheduler* S= nullptr;  //스케줄러

		typedef struct _stats {
			int finished_count; //완료한 작업
			int unfinished_count;  //완료하지 못한 작업
			int to_do_count;  //앞으로 남은 작업의 개수
			int total_task_count; //현재까지 추가된 작업의 개수 (지운거 제외)
			int tasktypes[100];   //작업의 종류를 저장하는 배열
		} calStats;   //통계 저장


		calStats stat = { 0,0,0,0, {0, } };

		void refreshCal();
		void remakeCal(std::vector<Task*>& newq, std::vector<Task*>& newf);
		cal_Day* find(int date);
		cal_Day* newDay(int date);

		int get_max_day(int year, int month);
		void get_next_day(int& year, int& month, int& day);
		void get_first_day_of_week(int & year, int & month, int & day);
		void get_current_time(int& year, int& month, int& day, int& hour, int& minute);

	public:
		Calender();
		~Calender();
		void save();
		void restore();
		bool addTask(std::string name, long long dur, long long duedate, int type);
		bool deleteTask(int taskNum);
		bool editTask(int taskNum, std::string name, long long dur, long long duedate, int type);
		bool editTask(int taskNum, std::string name, long long startime, long long endtime, int type, bool isFixed);
		void markFinished(int taskNum);
		bool addFixedTask(std::string name, long long startime, long long endtime, int type);
		void setUninterruptedTime(long long start, long long end);

		bool get_Day(std::vector<Task*> & tasks, int year, int month, int day);
		bool get_Week(std::vector<Task*> & tasks, int year, int month, int day);
		bool get_Month(std::vector<Task*> & tasks, int year, int month, int day);

		void getQueuedTasks(std::vector<Task*>& tasks); // 모든 작업 목록을 가져오는 함수
		calStats getStatistics();

		bool changeInterval(int interval, std::vector<Task*> & queued, std::vector<Task*> & failed);  //새로운 queued하고 failed 반환해줌.
		std::string task_type[10] = {  //여기에 Task 종류 추가
		"과제",
		"약속",
		"기타"
		};

	};
}