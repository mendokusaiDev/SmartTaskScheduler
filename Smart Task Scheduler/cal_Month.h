#pragma once
#include "Set.h"
#include <map>
//#include "cal_Day.h"


namespace scheduler {

	class cal_Day;
	/*template<class Item>
	class Set;*/

	class cal_Month {
	private:
		int year;  //¿¬µµ
		int month;  //¿ù
		Set<cal_Day*> day_headptr;

	public:
		cal_Day* get_Day(int day);
		void set_Day(int day);
		cal_Month(int year, int month);
		bool operator==(int n);
		bool operator==(cal_Month d) const;
	};
}