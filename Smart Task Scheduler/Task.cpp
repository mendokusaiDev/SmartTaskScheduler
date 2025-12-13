#include "Task.h"
#include "saveFile.h"
#include "ctime"
using namespace scheduler;

namespace scheduler {
	//task °ü·Ã
	Task::Task(std::string name, long long enddate, long long duration, int type, int taskNum) {
		this->name = name;
		this->enddate = enddate;
		this->duration = duration;
		this->type = type;
		this->starttime = -1;
		this->TaskNum = taskNum;
		this->finished = 0;
		this->endtime = -1;
	}

	Task::Task(bool fixed, std::string name, long long starttime, long long endtime, long long duration, int type, int taskNum) {
		this->name = name;
		this->starttime = starttime;
		this->endtime = endtime;
		this->type = type;
		this->TaskNum = taskNum;
		this->finished = 0;
		this->enddate = endtime;
		this->fixed = 1;
		this->duration = duration;
		
	}

	Task::Task(std::string name, saveFile* f) {
		this->name = name;
		this->enddate = f->enddate;
		this->duration = f->duration;
		this->type = f->type;
		this->starttime = f->starttime;
		this->endtime = f->endtime;
		this->TaskNum = f->TaskNum;
		this->finished = f->finished;
		this->fixed = f->fixed;

	}

	bool Task::isFixed() {
		return this->fixed;
	}

	void Task::getTime(long long& starttime, long long& endtime) {
		starttime = this->starttime;
		endtime = this->endtime;
		return;
	}


	void Task::changeTask(std::string name, long long enddate, long long duration, int type) {
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