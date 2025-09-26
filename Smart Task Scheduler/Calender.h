#pragma once
#include "Scheduler.h"
#include "node.h"
#include <map>
#include <vector>
#include <string>

using namespace std;


namespace scheduler {

	class Task {
	private:
		string name;  //이름
		int enddate;   //마감 날짜 (년월일시분)
		int duration;  //예상 소요 시간
		int type;      //작업 종류
		int starttime;  //시작 시간
		int endtime;    //끝 시간
		int TaskNum;  //고유 작업 번호
		bool finished = 0; //완료 여부
		
	public:
		Task(string name, int enddate, int duration, int type, int taskNum);
		void changeTask(string name, int enddate, int duration, int type);
		int getTaskNum();
	};
	
	
	class cal_Year : node {
	private:
		int year;  //연도
		node* next;  //다음 연도
		node* month_head;  //그 연도의 month head 포인터
		int month_count;   //그 연도에서 만들어진 month의 개수

	public:
		void link(node * next);
		void setData(node* data);

		cal_Year(int year);
	};

	class cal_Month : node {
	private:
		int year;  //연도
		int month;  //월
		node* next;  //다음 월
		node* day_head;   //그 월의 day head 포인터
		int day_count;  //만들어진 day 의 개수

	public:
		void link(node* next);
		void setData(node* data);

		cal_Month(int year, int month);
	};

	class cal_Day : node {
	private:
		int year; //연도
		int month;  //월
		int day;   //일
		node* next;   //다음 일
		map<int, Task*> saved;  //해야되는 일정의 번호
		

	public:
		void link(node* next);
		cal_Day(int year, int month, int day);
	};



	class Calender {
	private:
		vector<int> queued; //일정에 할당 된 작업
		vector<int> failed;  //일정 할당에 실패한 작업
		vector<int> finished;  //끝난 일정
		map<int, Task*> allTasks;  //모든 작업
		cal_Year* headptr;  //연도 headptr
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

	public:
		Calender();

		bool addTask(string name, int dur, int duedate, int type);
		bool deleteTask(int taskNum);
		bool editTask(int taskNum, string name, int dur, int duedate, int type);
		void markFinished(int taskNum);

		bool get_Day(vector<Task*> tasks, int year, int month, int day);
		bool get_Week(vector<Task*> tasks, int year, int month, int day);
		bool get_Month(vector<Task*> tasks, int year, int month, int day);
		calStats getStatistics();

		bool changeInterval(int interval, vector<Task*> queued, vector<Task*> failed);

	};
}