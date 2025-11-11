#pragma once
#include "Calender.h"
#include <iostream>

namespace scheduler {

	class TUI {

	private:
		Calender* c;

		void showMonth(int year, int month);
		void showWeek(int year, int month, int day);
		void showDay(int year, int month, int day);

		void addTask();
		void editTask();
		void deleteTask();

		void showStatistics();
		
		void changeInterval();

		void get_current_time(int& year, int& month, int& day);
		void get_next_day(int& year, int& month, int& day, int offset);
		void get_next_week(int& year, int& month, int& day, int offset);
		void get_next_month(int& year, int& month, int& day, int offset);



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