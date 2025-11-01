#include "cal_Month.h"


namespace scheduler {
	////////////////////////////////////////////////
	//cal Month °ü·Ã
	cal_Day* cal_Month::get_Day(int day) {
		if (this->day_headptr.find(day) == this->day_headptr.end()) {
			return nullptr;
		}

		return this->day_headptr[day];
	}

	void cal_Month::set_Day(int day) {

		if (get_Day(day) != nullptr) {
			std::cout << "error: " << day << "on " << this->month << "already exists\n";
			return;
		}

		this->day_headptr[day] = new cal_Day(this->year, this->month, day);
		return;

	}


	cal_Month::cal_Month(int year, int month) {
		this->year = year;
		this->month = month;
	}
}