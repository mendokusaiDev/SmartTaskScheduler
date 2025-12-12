#pragma once
#include <type_traits>
//#include "Task.h"
//#include "Calender.h"
//#include "cal_Day.h"
//#include "cal_Month.h"
//#include "cal_Year.h"


namespace scheduler {

	class cal_Day;
	class cal_Month;
	class cal_Year;

	template<class Item>
	class node {
	protected:
		Item data;
		node<Item>* next;
		//int task_num;
	public:
		void setLink(node<Item>* next) {
			this->next = next;
		}
		node<Item>* getLink() {
			return this->next;
		}
		void setData(Item data) {
			this->data = data;
		}
		Item getData() {
			return data;
		}
		//int get_tasknum() { return task_num; }
		node(Item task) {
			this->data = task;
			this->next = nullptr;
		}

		bool operator==(Item t) const{
			if (std::is_pointer<Item>::value) {
				return *data == *t;
			}
			return false;
		}
	};
}