#include "cal_Year.h"
#include "cal_Month.h"
#include "Set.h"

namespace scheduler {
	///////////////////////////////////
	//cal Year 관련

	bool cal_Year::operator==(int n) {
		if (this->year == n) return true;
		return false;
	}

	bool cal_Year::operator==(cal_Year d) const {
		if (this->year == d.year) return true;
		return false;
	}

	cal_Month* cal_Year::get_Month(int month) {
		return (cal_Month*)month_headptr.get_Item(month);
	}

	void cal_Year::set_Month(int month) {

		if (get_Month(month) != nullptr) {
			std::cout << "error: " << month << "on " << this->year << "already exists\n";
			return;
		}

		month_headptr.put_Item(new cal_Month(this->year, month)); //수정하기
	}

	cal_Year::cal_Year(int year) {
		this->year = year;
	}
}