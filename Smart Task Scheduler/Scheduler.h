#pragma once
#include <vector>

// [수정됨] 올바른 전방 선언 방식
// namespace 내부에서 선언하거나, namespace 블록으로 감싸야 합니다.
namespace scheduler {
    class Task;
    class PriorityQueue;
}

namespace scheduler {

    class Scheduler {

    private:
        long long interval;
        long long unInterruptStart = 0;
        long long unInterruptEnd = 0;

    public:
        // 전방 선언 덕분에 포인터 변수 사용 가능
        PriorityQueue* q = nullptr;

        bool ChangeInterval(long long time);

        long long get_interval() { return interval; }

        void setUnterruptedTime(long long start, long long end);

        // 함수 선언부에서는 Task* 포인터만 쓰므로 Task의 구체적 정의가 없어도 됨
        void makeSchedule(std::vector<Task*>& original, std::vector<Task*>& failed);

        // 생성자
        Scheduler(long long time) { this->interval = time; }
    };
}