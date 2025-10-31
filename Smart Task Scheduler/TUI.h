#pragma once
#include "Calender.h"

namespace scheduler {

	class TUI {

	private:
		Calender* c;

		void showMonth();
		void showWeek();
		void showDay();

		void addTask();
		void editTask();
		void deleteTask();

		void showStatistics();

		void changeInterval();

	public:
		void mainMenu();

	};



}