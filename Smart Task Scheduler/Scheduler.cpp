#define _CRT_SECURE_NO_WARNINGS

#include "Scheduler.h"      
#include "Priority_queue.h" 
#include "Task.h"            

#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <algorithm> 

namespace scheduler {

    typedef long long ll;

    namespace {
        static int max_day_of_month[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

        int get_max_day(int year, int month) {
            if (month != 2) return max_day_of_month[month];
            if (year % 100 == 0 && year % 400 != 0) return 28;
            if (year % 4 == 0) return 29;
            return 28;
        }

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

        bool isOverlapping(ll start1, ll end1, ll start2, ll end2) {
            return std::max(start1, start2) < std::min(end1, end2);
        }
    }

    bool Scheduler::ChangeInterval(long long time) {
        this->interval = time;
        return true;
    }

    void Scheduler::setUnterruptedTime(long long start, long long end) {
        this->unInterruptStart = start;
        this->unInterruptEnd = end;
    }

    void Scheduler::makeSchedule(std::vector<Task*>& original, std::vector<Task*>& failed) {
        if (!q) q = new PriorityQueue();
        q->clear();

        std::vector<Task*> fixedTasks;

        for (Task* task : original) {
            // [중요 수정] 시간이 있어도 Type이 1(고정)이 아니면 재스케줄링 해야 함
            // Task 클래스에 getType() 함수가 있다고 가정합니다.
            if (task->isFixed() == 1) {
                fixedTasks.push_back(task);
            }
            else {
                // 재스케줄링 대상이므로 기존 시간은 무시하고 큐에 넣음
                q->push(task);
            }
        }

        // 스케줄링 시작 시간 (현재 KST + 인터벌)
        time_t currentTimeT = std::time(NULL) + 9 * 60 * 60 + this->interval * 60;
        std::vector<Task*> scheduledTasks;

        // 고정 일정 먼저 포함
        for (auto t : fixedTasks) scheduledTasks.push_back(t);

        while (!q->empty()) {
            Task* currentTask = q->front();
            ll taskDuration = currentTask->getDuration();
            ll taskEndDate = currentTask->getEndDate();
            bool validSlotFound = false;

            while (!validSlotFound) {
                time_t estimatedEndTimeT = currentTimeT + taskDuration * 60;
                ll currentStartLL = ConvTtoLL(currentTimeT);
                ll currentEndLL = ConvTtoLL(estimatedEndTimeT);

                // 마감일 체크
                if (currentEndLL > taskEndDate) {
                    q->pop();
                    failed.push_back(currentTask);
                    validSlotFound = true;
                    continue;
                }

                bool collisionDetected = false;

                // 방해 금지 시간 체크
                if (this->unInterruptStart != this->unInterruptEnd) {
                    std::tm tms = *std::gmtime(&currentTimeT);
                    long long curHHMM = tms.tm_hour * 100 + tms.tm_min;
                    long long start = this->unInterruptStart;
                    long long end = this->unInterruptEnd;
                    bool isInUninterrupted = false;

                    if (start < end) { if (curHHMM >= start && curHHMM < end) isInUninterrupted = true; }
                    else { if (curHHMM >= start || curHHMM < end) isInUninterrupted = true; }

                    if (isInUninterrupted) {
                        collisionDetected = true;
                        int targetHour = end / 100;
                        int targetMin = end % 100;
                        std::tm jumpTm = *std::gmtime(&currentTimeT);
                        if ((jumpTm.tm_hour * 100 + jumpTm.tm_min) >= end) jumpTm.tm_mday += 1;
                        jumpTm.tm_hour = targetHour; jumpTm.tm_min = targetMin; jumpTm.tm_sec = 0;
                        currentTimeT = std::mktime(&jumpTm);
                    }
                }

                // 고정 일정 충돌 체크
                if (!collisionDetected) {
                    for (Task* fTask : fixedTasks) {
                        long long fStart, fEnd;
                        fTask->getTime(fStart, fEnd);
                        if (isOverlapping(currentStartLL, currentEndLL, fStart, fEnd)) {
                            collisionDetected = true;
                            // 고정 일정 끝 시간 파싱해서 점프
                            int f_year = fEnd / 100000000;
                            int f_month = (fEnd % 100000000) / 1000000;
                            int f_day = (fEnd % 1000000) / 10000;
                            int f_hour = (fEnd % 10000) / 100;
                            int f_min = fEnd % 100;
                            std::tm f_tm = { 0 };
                            f_tm.tm_year = f_year - 1900; f_tm.tm_mon = f_month - 1; f_tm.tm_mday = f_day;
                            f_tm.tm_hour = f_hour; f_tm.tm_min = f_min; f_tm.tm_isdst = -1;

                            time_t fixedEndTimeT = std::mktime(&f_tm);
                            currentTimeT = fixedEndTimeT + (this->interval * 60) + 9*60*60;
                            break;
                        }
                    }
                }

                if (!collisionDetected) {
                    q->pop();
                    currentTask->setTime(currentStartLL, currentEndLL);
                    scheduledTasks.push_back(currentTask);
                    currentTimeT = estimatedEndTimeT + (this->interval * 60);
                    validSlotFound = true;
                }
            }
        }
        original = scheduledTasks;
    }
}