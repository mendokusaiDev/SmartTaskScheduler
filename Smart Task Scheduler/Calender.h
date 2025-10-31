#pragma once
#include "Scheduler.h"
#include "Node.h"
#include <map>
#include <vector>
#include <string>

using namespace std;


namespace scheduler {

	#define OTHER 0
	#define SCHOOL_HW 1

	string task_type[10] = {  //여기에 Task 종류 추가
		"Other",
		"School Homework"
	};


	class Task {
	private:
		string name;  //이름
		int enddate;   //마감 날짜 (년월일시분)
		int duration;  //예상 소요 시간
		int type;      //작업 종류
		long long starttime;  //시작 시간
		long long endtime;    //끝 시간
		int TaskNum;  //고유 작업 번호
		bool finished = 0; //완료 여부
		
	public:
		Task(string name, int enddate, int duration, int type, int taskNum);
		void changeTask(string name, int enddate, int duration, int type);
		int getTaskNum();
		void setTime(long long starttime, long long endtime);
		void getTime(long long& starttime, long long& endtime);
		bool isfinished() {
			return finished;
		}
		void done() {
			finished = 1;
		}
		void resetTime();
		int getEndDate() const { return enddate; }
    	int getDuration() const { return duration; }
		int getType() {
			return type;
		}
		//int getEnddate() {
		//	return enddate;
		//}

	};
	
	
	class cal_Year{
	private:
		int year;  //연도
		map<int, cal_Month*> month_headptr;

	public:
		cal_Month* get_Month(int month);
		void set_Month(int month);
		cal_Year(int year);
	};

	class cal_Month{
	private:
		int year;  //연도
		int month;  //월
		map<int, cal_Day*> day_headptr;

	public:
		cal_Day* get_Day(int day);
		void set_Day(int day);
		cal_Month(int year, int month);
	};

	class cal_Day{
	private:
		int year; //연도
		int month;  //월
		int day;   //일
		node* Task_headptr;
		
	public:
		void insert_Task(int tasknum, Task* task);
		vector<Task*> get_Tasks();
		void freeTask();
		cal_Day(int year, int month, int day);
		void freeTaskNum(int tasknum);
	};



	class Calender {
	private:
		vector<int> queued; //일정에 할당 된 작업
		vector<int> failed;  //일정 할당에 실패한 작업
		vector<int> finished;  //끝난 일정
		map<int, Task*> allTasks;  //모든 작업
		//cal_Year* headptr;  //연도 headptr
		map<int, cal_Year*> year_headptr; //연도 headptr
		int tasks_count = 0;  //누적 일정 개수 (이게 일정의 고유번호가 됨)
		Scheduler* S;  //스케줄러

		typedef struct _stats {
			int finished_count; //완료한 작업
			int unfinished_count;  //완료하지 못한 작업
			int to_do_count;  //앞으로 남은 작업의 개수
			int tasktypes[100];   //작업의 종류를 저장하는 배열
		} calStats;   //통계 저장


		calStats stat = { 0,0,0,{0, } };

		void refreshCal();
		void remakeCal(vector<Task*>& newq, vector<Task*>& newf);
		cal_Day* find(int date);
		cal_Day* newDay(int date);

		int get_max_day(int year, int month);
		void get_next_day(int& year, int& month, int& day);
		void get_first_day_of_week(int & year, int & month, int & day);
		void get_current_time(int& year, int& month, int& day, int& hour, int& minute);

	public:
		Calender();

		bool addTask(string name, int dur, int duedate, int type);
		bool deleteTask(int taskNum);
		bool editTask(int taskNum, string name, int dur, int duedate, int type);
		void markFinished(int taskNum);

		bool get_Day(vector<Task*> & tasks, int year, int month, int day);
		bool get_Week(vector<Task*> & tasks, int year, int month, int day);
		bool get_Month(vector<Task*> & tasks, int year, int month, int day);
		calStats getStatistics();

		bool changeInterval(int interval, vector<Task*> & queued, vector<Task*> & failed);  //새로운 queued하고 failed 반환해줌.

	};
}