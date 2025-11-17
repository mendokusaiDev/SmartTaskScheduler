#include "saveFile.h"
#include "Task.h"

namespace scheduler {


	saveFile::saveFile(Task* t) {
		enddate = t->getEndDate();
		duration = t->getDuration();
		type = t->getType();
		starttime = t->getstarttime();
		endtime = t->getendtime();
		TaskNum = t->getTaskNum();
		finished = t->isfinished();
		fixed = t->isFixed();
	}
}