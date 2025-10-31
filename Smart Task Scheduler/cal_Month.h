#pragma once
#include <map>
#include "cal_Day.h"


namespace scheduler {

	class cal_Day;

	class cal_Month {
	private:
		int year;  //¿¬µµ
		int month;  //¿ù
		std::map<int, cal_Day*> day_headptr;

	public:
		cal_Day* get_Day(int day);
		void set_Day(int day);
		cal_Month(int year, int month);
	};
}