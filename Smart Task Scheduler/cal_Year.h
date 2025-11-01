#pragma once
#include "cal_Month.h"
#include <iostream>
#include <map>



namespace scheduler {

	class cal_Month;

	class cal_Year {
	private:
		int year;  //¿¬µµ
		std::map<int, cal_Month*> month_headptr;

	public:
		cal_Month* get_Month(int month);
		void set_Month(int month);
		cal_Year(int year);
	};
}