#include "cal_Month.h"
#include "cal_Day.h"
#include "Set.h"
#include <iostream>

namespace scheduler {
	////////////////////////////////////////////////
	//cal Month °ü·Ã

	bool cal_Month::operator==(int n) {
		if (this->month == n) return true;
		return false;
	}

	bool cal_Month::operator==(cal_Month d) const {
		if (this->month == d.month) return true;
		return false;
	}

	cal_Day* cal_Month::get_Day(int day) {
		return day_headptr.get_Item(day);
	}

	void cal_Month::set_Day(int day) {

		if (get_Day(day) != nullptr) {
			std::cout << "error: " << day << "on " << this->month << "already exists\n";
			return;
		}

		day_headptr.put_Item(new cal_Day(this->year, this->month, day));
		return;

	}


	cal_Month::cal_Month(int year, int month) {
		this->year = year;
		this->month = month;
	}
}