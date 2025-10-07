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

	string task_type[10] = {  //���⿡ Task ���� �߰�
		"Other",
		"School Homework"
	};


	class Task {
	private:
		string name;  //�̸�
		int enddate;   //���� ��¥ (����Ͻú�)
		int duration;  //���� �ҿ� �ð�
		int type;      //�۾� ����
		int starttime;  //���� �ð�
		int endtime;    //�� �ð�
		int TaskNum;  //���� �۾� ��ȣ
		bool finished = 0; //�Ϸ� ����
		
	public:
		Task(string name, int enddate, int duration, int type, int taskNum);
		void changeTask(string name, int enddate, int duration, int type);
		int getTaskNum();
		void setTime(int starttime, int endtime);
		void resetTime();

	};
	
	
	class cal_Year{
	private:
		int year;  //����
		map<int, cal_Month*> month_headptr;

	public:
		cal_Month* get_Month(int month);
		void set_Month(int month);
		cal_Year(int year);
	};

	class cal_Month{
	private:
		int year;  //����
		int month;  //��
		map<int, cal_Day*> day_headptr;

	public:
		cal_Day* get_Day(int day);
		void set_Day(int day);
		cal_Month(int year, int month);
	};

	class cal_Day{
	private:
		int year; //����
		int month;  //��
		int day;   //��
		node* Task_headptr;
		
	public:
		void insert_Task(int tasknum, Task* task);
		vector<Task*> get_Tasks();
		void freeTask();
		cal_Day(int year, int month, int day);
	};



	class Calender {
	private:
		vector<int> queued; //������ �Ҵ� �� �۾�
		vector<int> failed;  //���� �Ҵ翡 ������ �۾�
		vector<int> finished;  //���� ����
		map<int, Task*> allTasks;  //��� �۾�
		//cal_Year* headptr;  //���� headptr
		map<int, cal_Year*> year_headptr; //���� headptr
		int tasks_count = 0;  //���� ���� ���� (�̰� ������ ������ȣ�� ��)
		Scheduler* S;  //�����ٷ�

		typedef struct _stats {
			int finished_count; //�Ϸ��� �۾�
			int unfinished_count;  //�Ϸ����� ���� �۾�
			int to_do_count;  //������ ���� �۾��� ����
			int tasktypes[100];   //�۾��� ������ �����ϴ� �迭
		} calStats;   //��� ����


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