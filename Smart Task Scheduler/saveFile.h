#pragma once
//#include "Task.h"

namespace scheduler {
	
	class Task;

	class saveFile {
	public:
		
		long long enddate = -1;   //마감 날짜 (년월일시분)
		long long duration = -1;  //예상 소요 시간
		int type = -1;      //작업 종류
		long long starttime = -1;  //시작 시간
		long long endtime = -1;    //끝 시간
		int TaskNum = -1;  //고유 작업 번호
		bool finished = 0; //완료 여부

		saveFile(Task* t);

		saveFile(){}
	};
}