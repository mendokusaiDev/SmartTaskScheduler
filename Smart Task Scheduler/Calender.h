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
	};
	
	
	class cal_Year : node {
	private:
		int year;  //����
		node* next;  //���� ����
		node* month_head;  //�� ������ month head ������
		int month_count;   //�� �������� ������� month�� ����

	public:
		void link(node * next);
		void setData(node* data);

		cal_Year(int year);
	};

	class cal_Month : node {
	private:
		int year;  //����
		int month;  //��
		node* next;  //���� ��
		node* day_head;   //�� ���� day head ������
		int day_count;  //������� day �� ����

	public:
		void link(node* next);
		void setData(node* data);

		cal_Month(int year, int month);
	};

	class cal_Day : node {
	private:
		int year; //����
		int month;  //��
		int day;   //��
		node* next;   //���� ��
		map<int, Task*> saved;  //�ؾߵǴ� ������ ��ȣ
		

	public:
		void link(node* next);
		cal_Day(int year, int month, int day);
	};



	class Calender {
	private:
		vector<int> queued; //������ �Ҵ� �� �۾�
		vector<int> failed;  //���� �Ҵ翡 ������ �۾�
		vector<int> finished;  //���� ����
		map<int, Task*> allTasks;  //��� �۾�
		cal_Year* headptr;  //���� headptr
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