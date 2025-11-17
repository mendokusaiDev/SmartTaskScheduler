#define _CRT_SECURE_NO_WARNINGS
#include "Scheduler.h"
#include <iostream>

#include<chrono>
#include<ctime>

namespace scheduler {
    
    typedef long long ll;

    namespace {

        //max_day_of_month
        static int max_day_of_month[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
        int get_max_day(int year, int month) {
            if (month != 2) return max_day_of_month[month];
            if (year % 100 == 0 && year % 400 != 0) return 28;
            if (year % 4 == 0) return 29;
            return 28;
        }

        // YYYY MM DD 
        int addDaysToDate(int date_yyyymmdd, int duration) {
            int year = date_yyyymmdd / 10000;
            int month = (date_yyyymmdd % 10000) / 100;
            int day = date_yyyymmdd % 100;

            // duration이 0일인 경우(오늘 시작, 오늘 마감)를 대비해
            for (int i = 0; i < duration; ++i) {
                day++;
                if (day > get_max_day(year, month)) {
                    day = 1;
                    month++;
                    if (month > 12) {
                        month = 1;
                        year++;
                    }
                }
            }
            return day + month * 100 + year * 10000;
        }

        int addDaysToDate(ll date_yyyymmdd, ll duration) {  //deprecated
            int year = date_yyyymmdd / 10000;
            int month = (date_yyyymmdd % 10000) / 100;
            int day = date_yyyymmdd % 100;

            // duration이 0일인 경우(오늘 시작, 오늘 마감)를 대비해
            for (int i = 0; i < duration; ++i) {
                day++;
                if (day > get_max_day(year, month)) {
                    day = 1;
                    month++;
                    if (month > 12) {
                        month = 1;
                        year++;
                    }
                }
            }
            return day + month * 100 + year * 10000;
        }

        // Calender.cpp의 get_current_time 로직에서
        // 날짜(YYYYMMDD) 부분만 정수로 반환하는 함수
        
        long long ConvTtoLL(time_t date) {
            std::tm tms = *std::gmtime(&date);
            ll year, month, day, hour, min;
            min = tms.tm_min;
            hour = tms.tm_hour * 100;
            day = tms.tm_mday * 10000;
            month = (tms.tm_mon + 1) * 1000000LL;
            year = (tms.tm_year+1900) * 100000000LL;
            
            return year + month + day + hour + min;
        }
    }
    //익명 namespace 추가 부분 종료 시점
    
void Scheduler::makeSchedule(std::vector<Task*>& original, std::vector<Task*>& failed) {
    // q 포인터가 초기화되지 않았다면 생성
    if (!q) {
        q = new PriorityQueue();
    }
    // 재사용을 위해 큐를 비웁니다.
    q->clear();

    // original 벡터의 모든 작업을 우선순위 큐에 넣습니다.
    for (Task* task : original) {
        q->push(task);
    }

    // 스케줄링을 위한 가상 시간  -> get_current_date_yyyymmdd()호출
    time_t currentTime = std::time(NULL) + 9*60*60 + this->interval*60;  //누적 초 시간

    std::vector<Task*> scheduledTasks; // 성공한 작업을 임시로 저장할 벡터
    
    // 큐가 빌 때까지 반복합니다.
    while (!q->empty()) {
        // 우선순위가 가장 높은 작업(마감일이 가장 빠른 작업)을 가져옵니다.
        Task* currentTask = q->front();
        q->pop();

        // 1단계에서 추가한 Getter 함수를 사용하여 정보를 가져옵니다.
        ll taskDuration = currentTask->getDuration(); 
        ll taskEndDate = currentTask->getEndDate();   

        // 단순 덧셈 -> 날짜 계산으로 수정

        time_t estimatedEndTime = currentTime + taskDuration * 60;  // 초 단위로 변환
        ll estimatedEndTimeLL = ConvTtoLL(estimatedEndTime);

        // 마감일 내에 작업을 완료할 수 있는지 확인합니다.
        if (estimatedEndTimeLL <= taskEndDate) {
            // 스케줄링 성공: 담당자가 만든 setTime 함수를 호출합니다.
            currentTask->setTime(ConvTtoLL(currentTime), estimatedEndTimeLL);
            
            // 성공 목록에 추가하고, 현재 시간을 업데이트합니다.
            scheduledTasks.push_back(currentTask);
            currentTime = estimatedEndTime + this->interval*60;
        } else {
            // 스케줄링 실패: 실패 목록으로 이동합니다.
            failed.push_back(currentTask);
        }
    }

    // 성공한 작업 목록으로 original 벡터를 덮어씁니다.
    original = scheduledTasks;
    
    // 진행 상황을 확인하기 위한 콘솔 출력 -> real time base로 수정
    //std::cout << "Scheduling complete using Priority Queue (Real-Time Basis)." << std::endl;    std::cout << "Succeeded: " << original.size() << " tasks" << std::endl;
    //std::cout << "Failed: " << failed.size() << " tasks" << std::endl;
}

// ChangeInterval 함수 (수정 없음)
bool Scheduler::ChangeInterval(long long time) {
    this->interval = time;
    return true;
}

void Scheduler::setUnterruptedTime(long long start, long long end) {
    this->unInterruptStart = start;
    this->unInterruptEnd = end;
}

} // namespace scheduler
