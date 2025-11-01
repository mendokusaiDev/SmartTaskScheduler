#pragma once
#include "Calender.h"
#include <iostream>

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
		TUI() {
			c = new Calender();
		};
		void atExit() {
			c->save();
			std::cout << "saved!" << std::endl;
		}

	};



}