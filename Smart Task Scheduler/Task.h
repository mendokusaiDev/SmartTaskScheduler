#pragma once
#include <string>
//#include "saveFile.h"
//using namespace std;

namespace scheduler {
	class saveFile;

	class Task {
	private:
		std::string name="";  //이름
		long long enddate=-1;   //마감 날짜 (년월일시분)
		long long duration=-1;  //예상 소요 시간
		int type=-1;      //작업 종류
		long long starttime=-1;  //시작 시간
		long long endtime=-1;    //끝 시간
		int TaskNum=-1;  //고유 작업 번호
		bool finished = 0; //완료 여부

	public:
		Task(std::string name, long long enddate, long long duration, int type, int taskNum);
		Task(std::string name, saveFile* f);
		void changeTask(std::string name, long long enddate, long long duration, int type);
		int getTaskNum();
		std::string getTaskName() { return name; }
		void setTime(long long starttime, long long endtime);
		void getTime(long long& starttime, long long& endtime);
		bool isfinished() {
			return finished;
		}
		void done() {
			finished = 1;
		}
		void resetTime();
		long long getEndDate() const { return enddate; }
		long long getDuration() const { return duration; }
		int getType() {
			return type;
		}
		long long getstarttime() { return starttime; }
		long long getendtime() { return endtime; }
		//int getEnddate() {
		//	return enddate;
		//}

	};
}