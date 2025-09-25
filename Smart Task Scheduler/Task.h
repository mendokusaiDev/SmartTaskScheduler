#pragma once
#include "LinkedList.h"
#include <iostream>

using namespace std;


namespace scheduler {
	class Task {
		/*
			멤버 변수: 일정 이름
					   일정 종류
			           고정 일정인지
					   일정 시작 시간
					   일정 끝 시간
					   일정 마감 일

			멤버 함수: 일정 만들기
			           일정 수정하기
					   일정 삭제하기
		*/

	private:
		string name;
		int type;
		int start_time;
		int end_time;
		int end_date;
		Task* next;

	public:
		void link(const Task* next);
		Task();
		Task(const Task& task);
		void editTask(const Task& task);


	};
}