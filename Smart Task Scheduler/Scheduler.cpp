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

        static constexpr int KST_OFFSET_SEC = 9 * 60 * 60;

        // UTC time_t -> tm (KST 달력값)
        static std::tm utc_to_kst_tm(time_t utc) {
            time_t shifted = utc + KST_OFFSET_SEC;
            return *std::gmtime(&shifted); // gmtime은 UTC 달력값을 뽑는데, shifted를 넣었으니 KST처럼 나옴
        }

        // KST 달력값(tm) -> UTC time_t
        static time_t kst_tm_to_utc(const std::tm& kst_tm) {
            std::tm tmp = kst_tm;
            tmp.tm_isdst = 0;
            // tmp를 "UTC 달력값"으로 보고 epoch으로 만든 뒤, KST 오프셋을 빼면 UTC가 됨
            return _mkgmtime(&tmp) - KST_OFFSET_SEC;
        }

        // UTC time_t -> YYYYMMDDhhmm (KST 기준 저장값)
        static long long utc_to_kst_ll(time_t utc) {
            std::tm t = utc_to_kst_tm(utc);
            long long year = (t.tm_year + 1900) * 100000000LL;
            long long month = (t.tm_mon + 1) * 1000000LL;
            long long day = t.tm_mday * 10000LL;
            long long hour = t.tm_hour * 100LL;
            long long min = t.tm_min;
            return year + month + day + hour + min;
        }

        // YYYYMMDDhhmm(KST 저장값) -> UTC time_t
        static time_t kst_ll_to_utc(long long yyyymmddhhmm) {
            int y = (int)(yyyymmddhhmm / 100000000);
            int m = (int)((yyyymmddhhmm % 100000000) / 1000000);
            int d = (int)((yyyymmddhhmm % 1000000) / 10000);
            int hh = (int)((yyyymmddhhmm % 10000) / 100);
            int mm = (int)(yyyymmddhhmm % 100);

            std::tm t{};
            t.tm_year = y - 1900;
            t.tm_mon = m - 1;
            t.tm_mday = d;
            t.tm_hour = hh;
            t.tm_min = mm;
            t.tm_sec = 0;
            t.tm_isdst = 0;

            return kst_tm_to_utc(t);
        }

        int get_max_day(int year, int month) {
            if (month != 2) return max_day_of_month[month];
            if (year % 100 == 0 && year % 400 != 0) return 28;
            if (year % 4 == 0) return 29;
            return 28;
        }

        long long ConvTtoLL(time_t utcDate) {
            return utc_to_kst_ll(utcDate); // 저장용은 항상 KST 기준
        }

        bool isOverlapping(ll start1, ll end1, ll start2, ll end2) {
            return std::max(start1, start2) < std::min(end1, end2);
        }

        int toMinute(long long hhmm) {
            return (hhmm / 100) * 60 + (hhmm % 100);
        }

        bool isOverlapWithUninterrupt(time_t startT, time_t endT,
            long long dndStart, long long dndEnd)
        {
            int dndS = toMinute(dndStart);
            int dndE = toMinute(dndEnd);

            std::tm ts = utc_to_kst_tm(startT);
            std::tm te = utc_to_kst_tm(endT);

            int sMin = ts.tm_hour * 60 + ts.tm_min;
            int eMin = te.tm_hour * 60 + te.tm_min;

            // ✅ 자정 넘김 판정은 tm_yday/tm_year로
            bool crossesMidnight =
                (ts.tm_year != te.tm_year) || (ts.tm_yday != te.tm_yday);
            if (crossesMidnight) eMin += 1440;

            auto overlap = [](int a1, int a2, int b1, int b2) {
                return std::max(a1, b1) < std::min(a2, b2);
                };

            if (dndS < dndE) {
                return overlap(sMin, eMin, dndS, dndE);
            }
            else {
                return overlap(sMin, eMin, dndS, 1440) ||
                    overlap(sMin, eMin, 1440, 1440 + dndE);
            }
        }


        time_t jumpToUninterruptEnd(time_t curUtc, long long dndEndHHMM)
        {
            int endMin = toMinute(dndEndHHMM);

            std::tm kst = utc_to_kst_tm(curUtc);
            int curMin = kst.tm_hour * 60 + kst.tm_min;

            if (curMin >= endMin) kst.tm_mday += 1;

            kst.tm_hour = endMin / 60;
            kst.tm_min = endMin % 60;
            kst.tm_sec = 0;

            return kst_tm_to_utc(kst); // ✅ UTC time_t로 복원
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

    void Scheduler::getUninterruptedTime(long long& start, long long& end) {
        start = this->unInterruptStart;
        end = this->unInterruptEnd;
        return;
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
        time_t currentTimeT = std::time(NULL) + this->interval * 60;
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
                    if (isOverlapWithUninterrupt(currentTimeT, estimatedEndTimeT,
                        this->unInterruptStart, this->unInterruptEnd))
                    {
                        collisionDetected = true;
                        currentTimeT = jumpToUninterruptEnd(currentTimeT, this->unInterruptEnd);
                        continue; 
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

                            time_t fixedEndUtc = kst_ll_to_utc(fEnd);
                            currentTimeT = fixedEndUtc + (this->interval * 60);

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