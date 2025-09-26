#pragma once
#include "Calender.h"

namespace scheduler {
	
	class TUI {
		/*
			멤버 함수: 메뉴 표시하기 및 기능 구현
			             - 일정 추가하기
						 - 일정 삭제하기
						 - 일정 변경하기
						 - 인터벌 변경하기
						 - 일정 보기
							- 월 단위
							- 주 단위
							- 일 단위
						
		*/

	private:
		Calender* c;
		void showMonth();
		void showWeek();
		void showDay();
		void addTask();
		void editTask();
		void changeInterval();
		void showStatistics();

	public:
		void mainMenu();
		TUI();
		TUI(Calender& c);
	};
}