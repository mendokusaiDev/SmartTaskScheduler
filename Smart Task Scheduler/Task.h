#pragma once
#include <string>
//using namespace std;

namespace scheduler {

	class Task {
	private:
		std::string name;  //이름
		long long enddate;   //마감 날짜 (년월일시분)
		long long duration;  //예상 소요 시간
		int type;      //작업 종류
		long long starttime;  //시작 시간
		long long endtime;    //끝 시간
		int TaskNum;  //고유 작업 번호
		bool finished = 0; //완료 여부

	public:
		Task(std::string name, long long enddate, long long duration, int type, int taskNum);
		void changeTask(std::string name, long long enddate, long long duration, int type);
		int getTaskNum();
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
		//int getEnddate() {
		//	return enddate;
		//}

	};
}