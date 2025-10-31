#include "cal_Year.h"

namespace scheduler {
	///////////////////////////////////
	//cal Year 관련
	cal_Month* cal_Year::get_Month(int month) {
		if (this->month_headptr.find(month) == this->month_headptr.end())
			return nullptr;

		return this->month_headptr[month];
	}


	void cal_Year::set_Month(int month) {

		if (get_Month(month) != nullptr) {
			std::cout << "error: " << month << "on " << this->year << "already exists\n";
			return;
		}

		this->month_headptr[month] = new cal_Month(this->year, month); //수정하기

	}

	cal_Year::cal_Year(int year) {
		this->year = year;
	}
}