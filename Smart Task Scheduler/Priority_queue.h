#pragma once
#include "Calender.h"
#include <vector> // std::vector를 사용하기 위해 추가

namespace scheduler {

class PriorityQueue {
private:
    // Task 포인터를 저장하고, 이 벡터를 기반으로 힙 자료구조를 구성
    std::vector<Task*> heap;
	void heapify_up(int index);   // 특정 노드를 부모와 비교하며 위로 올리는 함수
    void heapify_down(int index); // 특정 노드를 자식과 비교하며 아래로 내리는 함수

public:
    // 큐 비우기.
    void clear();
    
    // 큐에 새로운 작업을 추가
    void push(Task* task);
    
    // 우선순위가 가장 높은 작업을 큐에서 제거
    void pop();
    
    // 우선순위가 가장 높은 작업을 반환 (제거하지 않음)
    Task* front();

    // 큐가 비어있는지 여부를 반환
    bool empty() const;
};

}