#include "Priority_queue.h"
#include <algorithm> // std::push_heap, std::pop_heap을 사용하기 위해 포함
#include <stdexcept> // 예외 처리를 위해 포함

namespace scheduler {

// 우선순위 큐 내부에서 Task들의 순서를 정하기 위한 비교 구조체
// 마감일(enddate)이 더 늦은 Task를 "덜 중요하다(less)"고 판단,
// 마감일이 가장 빠른 Task가 힙의 맨 앞에 오도록(min-heap)
struct TaskComparator {
    bool operator()(const Task* a, const Task* b) const {
        // a의 마감일이 b의 마감일보다 늦으면 true를 반환 (a의 우선순위가 낮음)
        return a->getEndDate() > b->getEndDate();
    }
};


void PriorityQueue::push(Task* task) {
    // 1. 벡터의 맨 뒤에 새로운 작업을 추가
    heap.push_back(task);
    // 2. push_heap 함수를 호출하여 벡터를 다시 힙 속성을 만족하도록 재정렬
    std::push_heap(heap.begin(), heap.end(), TaskComparator());
}

void PriorityQueue::pop() {
    if (empty()) {
        // 비어있는 큐에서 pop을 시도하면 예외를 발생시키거나, 아무것도 하지 않을 수 있습니다.
        // 여기서는 아무것도 하지 않도록 처리합니다.
        return;
    }
    // 1. pop_heap 함수는 우선순위가 가장 높은 원소(첫 번째 원소)를 벡터의 맨 뒤로
    std::pop_heap(heap.begin(), heap.end(), TaskComparator());
    // 2. 벡터의 맨 뒤로 이동한 원소를 실제로 제거
    heap.pop_back();
}

Task* PriorityQueue::front() {
    if (empty()) {
        // 비어있는 경우, 접근하면 위험하므로 nullptr을 반환하거나 예외를 발생
        // 여기서는 nullptr을 반환
        return nullptr;
    }
    // 힙의 첫 번째 원소가 항상 우선순위가 가장 높은 원소입니다.
    return heap.front();
}

void PriorityQueue::clear() {
    heap.clear();
}

bool PriorityQueue::empty() const {
    return heap.empty();
}

} // namespace scheduler