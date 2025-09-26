#pragma once
#include "LinkedList.h"
#include "node.h"

namespace scheduler {

	class cal_Year : node{
		/*
		멤버 변수: cal_Month의 headptr을 저장

		멤버 함수: 년도 추가하기
				   월 추가하기
		*/

	private:
		node* month;
		node* next;
		int year;
		int month_count;

	public:
		const cal_Year* findYear(int year);
		void createYear(int year);
		void link();
		
		
	};
	
	class cal_Month {
		/*
		멤버 변수: cal_Day의 headptr을 저장

		멤버 함수: 월 추가하기
		           일 추가하기
		*/

	private:
		node* day;
		node* schedule;
		int month;
		int year;

	public:
		void createMonth(int month);
		const cal_Month* findMonth(int month);
		void link();

	};

	class cal_Day {
		/*
		멤버 변수: 하루 일정 (그 하루에 해당하는 Task 를 LinkedList 형식으로 저장)

		멤버 함수: 일정 할당하기
		           일정 리셋하기
		*/

	};

	class cal_Week {


	};

	class Calender {
		/*
		멤버 변수: cal_Year의 headptr을 저장
		           통계 저장
				   완료된 일정 저장 배열
				   미완료된 일정 저장 배열

				 

		멤버 함수: 일정 추가하기
				   일정 삭제하기
				   일정 인터벌 바꾸기
				   월 단위 주요 일정 정보 가져오기
				   주 단위 일정 정보 가져오기
				   일 단위 일정 정보 가져오기
				   일정 변경하기
				   통계 불러오기
		
		*/

	private:


	public:

		void addTask();
		void deleteTask();
		void editTask();
		void changeInterval();
		void changeSchedule();
		cal_Day get_Day();
		cal_Month get_Month();
		cal_Week get_Week();
		void getStatistics();
		Calender();
	};
}