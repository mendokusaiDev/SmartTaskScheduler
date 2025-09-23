#pragma once

namespace scheduler {

	class cal_Year {
		/*
		멤버 변수: cal_Month의 headptr을 저장

		멤버 함수: 년도 추가하기
				   월 추가하기
		*/

	};
	
	class cal_Month {
		/*
		멤버 변수: cal_Day의 headptr을 저장

		멤버 함수: 월 추가하기
		           일 추가하기
		*/

	};

	class cal_Day {
		/*
		멤버 변수: 하루 일정 (그 하루에 해당하는 Task 를 LinkedList 형식으로 저장)

		멤버 함수: 일정 할당하기
		           일정 리셋하기
		*/

	};

	class Calender {
		/*
		멤버 변수: cal_Year의 headptr을 저장
		           통계 저장

				 

		멤버 함수: 일정 추가하기
				   일정 삭제하기
				   일정 인터벌 바꾸기
				   월 단위 주요 일정 정보 가져오기
				   주 단위 일정 정보 가져오기
				   일 단위 일정 정보 가져오기
				   일정 변경하기
				   통계 불러오기
		
		*/

	};
}