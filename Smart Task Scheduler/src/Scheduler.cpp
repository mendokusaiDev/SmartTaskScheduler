#include "../Scheduler.h"
#include "../Priority_queue.h" 
#include <vector>
#include <iostream>

namespace scheduler {

// Scheduler.h에 선언된 PriorityQueue* q를 사용
// 소멸자에서 메모리 해제를 처리해야 합니다.
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

    // 스케줄링을 위한 가상 시간 (0부터 시작)
    int currentTime = 0; 
    std::vector<Task*> scheduledTasks; // 성공한 작업을 임시로 저장할 벡터
    
    // 큐가 빌 때까지 반복합니다.
    while (!q->empty()) {
        // 우선순위가 가장 높은 작업(마감일이 가장 빠른 작업)을 가져옵니다.
        Task* currentTask = q->front();
        q->pop();

        // 1단계에서 추가한 Getter 함수를 사용하여 정보를 가져옵니다.
        int taskDuration = currentTask->getDuration(); 
        int taskEndDate = currentTask->getEndDate();   

        // 현재 시간부터 작업을 수행했을 때의 예상 종료 시간
        int estimatedEndTime = currentTime + taskDuration;

        // 마감일 내에 작업을 완료할 수 있는지 확인합니다.
        if (estimatedEndTime <= taskEndDate) {
            // 스케줄링 성공: 담당자가 만든 setTime 함수를 호출합니다.
            currentTask->setTime(currentTime, estimatedEndTime);
            
            // 성공 목록에 추가하고, 현재 시간을 업데이트합니다.
            scheduledTasks.push_back(currentTask);
            currentTime = estimatedEndTime;
        } else {
            // 스케줄링 실패: 실패 목록으로 이동합니다.
            failed.push_back(currentTask);
        }
    }

    // 성공한 작업 목록으로 original 벡터를 덮어씁니다.
    original = scheduledTasks;
    
    // 진행 상황을 확인하기 위한 콘솔 출력
    std::cout << "Scheduling complete using Priority Queue." << std::endl;
    std::cout << "Succeeded: " << original.size() << " tasks" << std::endl;
    std::cout << "Failed: " << failed.size() << " tasks" << std::endl;
}

// ChangeInterval 함수 (수정 없음)
bool Scheduler::ChangeInterval(int time) {
    this->interval = time;
    return true;
}

} // namespace scheduler