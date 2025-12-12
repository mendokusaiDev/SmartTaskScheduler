#pragma once
#include <iostream>
#include <map>
#include "Set.h"
//#include "cal_Month.h"


namespace scheduler {

	class cal_Month;
	/*template<class Item>
	class Set;*/

	class cal_Year {
	private:
		int year;  //¿¬µµ
		Set<cal_Month*> month_headptr;

	public:
		cal_Month* get_Month(int month);
		void set_Month(int month);
		cal_Year(int year);
		bool operator==(int n);
		bool operator==(cal_Year d) const;
	};
}