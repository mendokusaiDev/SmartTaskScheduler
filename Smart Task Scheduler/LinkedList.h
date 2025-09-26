#pragma once
#include "Calender.h"

namespace scheduler {

	static class LinkedList {
	public:
		static void add_node(cal_Year* headptr, int year);
		static void add_node(cal_Month* headptr, int month);
		static void add_node(cal_Day* headptr, int day);

		static cal_Year* search_node(cal_Year* headptr, int year);
		static cal_Month* search_node(cal_Year* headptr, int year, int month);
		static cal_Day* search_node(cal_Year* headptr, int year, int month, int day);

	};
}