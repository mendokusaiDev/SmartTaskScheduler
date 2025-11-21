#define _CRT_SECURE_NO_WARNINGS

#include "Scheduler.h"      
#include "Priority_queue.h" 
#include "Task.h"            

#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <algorithm> // std::max, std::min

namespace scheduler {

    typedef long long ll;

    namespace {
        // --- 내부 헬퍼 함수들 ---

        static int max_day_of_month[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

        int get_max_day(int year, int month) {
            if (month != 2) return max_day_of_month[month];
            if (year % 100 == 0 && year % 400 != 0) return 28;
            if (year % 4 == 0) return 29;
            return 28;
        }

        // time_t를 YYYYMMDDHHMM (long long) 형식으로 변환
        long long ConvTtoLL(time_t date) {
            std::tm tms = *std::gmtime(&date);

            ll year, month, day, hour, min;
            min = tms.tm_min;
            hour = tms.tm_hour * 100;
            day = tms.tm_mday * 10000;
            month = (tms.tm_mon + 1) * 1000000LL;
            year = (tms.tm_year + 1900) * 100000000LL;

            return year + month + day + hour + min;
        }

        // 두 구간 [start1, end1] 과 [start2, end2] 가 겹치는지 확인하는 함수
        bool isOverlapping(ll start1, ll end1, ll start2, ll end2) {
            return std::max(start1, start2) < std::min(end1, end2);
        }
    }

    // --- Scheduler 클래스 구현 ---

    bool Scheduler::ChangeInterval(long long time) {
        this->interval = time;
        return true;
    }

    void Scheduler::setUnterruptedTime(long long start, long long end) {
        this->unInterruptStart = start;
        this->unInterruptEnd = end;
    }

    void Scheduler::makeSchedule(std::vector<Task*>& original, std::vector<Task*>& failed) {
        // 1. 우선순위 큐 초기화
        if (!q) {
            q = new PriorityQueue();
        }
        q->clear();

        // 2. 고정 일정(Fixed)과 유동 일정(Floating) 분리
        std::vector<Task*> fixedTasks;

        for (Task* task : original) {
            long long t_start, t_end;

            // Task.h를 include 했으므로 Task 멤버 함수 접근 가능
            task->getTime(t_start, t_end);

            if (t_start != -1 && t_end != -1) {
                // 이미 시간이 정해진 고정 일정
                fixedTasks.push_back(task);
            }
            else {
                // 스케줄링이 필요한 유동 일정
                q->push(task);
            }
        }

        // 3. 스케줄링 시작 시간 설정 (현재 시간 + 9시간(KST) + 초기 인터벌)
        time_t currentTimeT = std::time(NULL) + 9 * 60 * 60 + this->interval * 60;

        std::vector<Task*> scheduledTasks;

        // 결과 벡터에 고정 일정 먼저 추가
        for (auto t : fixedTasks) {
            scheduledTasks.push_back(t);
        }

        // 4. 큐가 빌 때까지 반복 (유동 일정 배치)
        while (!q->empty()) {
            Task* currentTask = q->front();

            ll taskDuration = currentTask->getDuration();
            ll taskEndDate = currentTask->getEndDate();

            bool validSlotFound = false;

            // 적절한 시간 슬롯을 찾을 때까지 반복
            while (!validSlotFound) {

                // A. 현재 고려 중인 시간 구간 계산
                time_t estimatedEndTimeT = currentTimeT + taskDuration * 60;
                ll currentStartLL = ConvTtoLL(currentTimeT);
                ll currentEndLL = ConvTtoLL(estimatedEndTimeT);

                // B. 마감일 초과 체크
                if (currentEndLL > taskEndDate) {
                    q->pop();
                    failed.push_back(currentTask);
                    validSlotFound = true;
                    continue;
                }

                bool collisionDetected = false;

                // C. 방해 금지 시간 체크
                if (this->unInterruptStart != this->unInterruptEnd) {
                    std::tm tms = *std::gmtime(&currentTimeT);
                    long long curHHMM = tms.tm_hour * 100 + tms.tm_min;
                    long long start = this->unInterruptStart;
                    long long end = this->unInterruptEnd;
                    bool isInUninterrupted = false;

                    if (start < end) {
                        if (curHHMM >= start && curHHMM < end) isInUninterrupted = true;
                    }
                    else {
                        if (curHHMM >= start || curHHMM < end) isInUninterrupted = true;
                    }

                    if (isInUninterrupted) {
                        collisionDetected = true;
                        // 방해 금지 시간이 끝나는 시간으로 점프
                        int targetHour = end / 100;
                        int targetMin = end % 100;
                        std::tm jumpTm = *std::gmtime(&currentTimeT);

                        if ((jumpTm.tm_hour * 100 + jumpTm.tm_min) >= end) {
                            jumpTm.tm_mday += 1;
                        }
                        jumpTm.tm_hour = targetHour;
                        jumpTm.tm_min = targetMin;
                        jumpTm.tm_sec = 0;

                        currentTimeT = std::mktime(&jumpTm);
                    }
                }

                // D. 고정 일정 충돌 체크
                if (!collisionDetected) {
                    for (Task* fTask : fixedTasks) {
                        long long fStart, fEnd;
                        fTask->getTime(fStart, fEnd);

                        if (isOverlapping(currentStartLL, currentEndLL, fStart, fEnd)) {
                            collisionDetected = true;

                            // 고정 일정 끝나는 시간 + 인터벌 만큼 점프
                            int f_year = fEnd / 100000000;
                            int f_month = (fEnd % 100000000) / 1000000;
                            int f_day = (fEnd % 1000000) / 10000;
                            int f_hour = (fEnd % 10000) / 100;
                            int f_min = fEnd % 100;

                            std::tm f_tm = { 0 };
                            f_tm.tm_year = f_year - 1900;
                            f_tm.tm_mon = f_month - 1;
                            f_tm.tm_mday = f_day;
                            f_tm.tm_hour = f_hour;
                            f_tm.tm_min = f_min;
                            f_tm.tm_isdst = -1;

                            time_t fixedEndTimeT = std::mktime(&f_tm);
                            currentTimeT = fixedEndTimeT + (this->interval * 60);

                            break;
                        }
                    }
                }

                // E. 충돌 없음 -> 확정
                if (!collisionDetected) {
                    q->pop();

                    currentTask->setTime(currentStartLL, currentEndLL);
                    scheduledTasks.push_back(currentTask);

                    currentTimeT = estimatedEndTimeT + (this->interval * 60);
                    validSlotFound = true;
                }
            }
        }

        // 5. 결과 반영
        original = scheduledTasks;
    }

} // namespace scheduler