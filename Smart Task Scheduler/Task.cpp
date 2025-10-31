#include "Task.h"

using namespace scheduler;

namespace scheduler {
	//task ฐüทร
	Task::Task(std::string name, int enddate, int duration, int type, int taskNum) {
		this->name = name;
		this->enddate = enddate;
		this->duration = duration;
		this->type = type;
		this->starttime = -1;
		this->TaskNum = taskNum;
		this->finished = 0;
		this->endtime = -1;
	}

	void Task::getTime(long long& starttime, long long& endtime) {
		starttime = this->starttime;
		endtime = this->endtime;
		return;
	}


	void Task::changeTask(std::string name, int enddate, int duration, int type) {
		this->name = name;
		this->enddate = enddate;
		this->duration = duration;
		this->type = type;
	}


	int Task::getTaskNum() {
		return this->TaskNum;
	}

	void Task::setTime(long long starttime, long long endtime) {
		this->starttime = starttime;
		this->endtime = endtime;
	}

	void Task::resetTime() {
		setTime(-1, -1);
	}
}