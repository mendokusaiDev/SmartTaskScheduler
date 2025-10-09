#include "Priority_queue.h"
#include <utility> // std::swap을 사용하기 위해 포함

namespace scheduler {

// --- Private Helper Functions ---

// 특정 노드(index)의 값이 부모 노드보다 우선순위가 높을 경우, 위로 올리는 작업을 반복합니다.
// 저희의 우선순위는 '마감일이 더 빠른 것'입니다. (min-heap)
void PriorityQueue::heapify_up(int index) {
    // 루트 노드(index 0)에 도달하거나, 부모보다 우선순위가 낮아질 때까지 반복
    while (index > 0) {
        int parent_index = (index - 1) / 2;

        // 자식의 마감일이 부모의 마감일보다 빠르면(우선순위가 높으면) 위치를 교체
        if (heap[index]->getEndDate() < heap[parent_index]->getEndDate()) {
            std::swap(heap[index], heap[parent_index]);
            index = parent_index; // 인덱스를 부모 위치로 변경하고 계속 진행
        } else {
            // 부모보다 우선순위가 낮으면 제자리를 찾은 것이므로 종료
            break;
        }
    }
}

// 특정 노드(index)의 값이 자식 노드보다 우선순위가 낮을 경우, 아래로 내리는 작업을 반복합니다.
void PriorityQueue::heapify_down(int index) {
    int left_child_index;
    int right_child_index;
    int smaller_child_index;

    // 자식 노드가 있는 동안 계속 반복
    while ((left_child_index = (2 * index) + 1) < heap.size()) {
        smaller_child_index = left_child_index; // 일단 왼쪽 자식이 더 작다고 가정
        right_child_index = (2 * index) + 2;

        // 오른쪽 자식이 존재하고, 오른쪽이 왼쪽보다 더 우선순위가 높으면(마감일이 빠르면)
        if (right_child_index < heap.size() && heap[right_child_index]->getEndDate() < heap[left_child_index]->getEndDate()) {
            smaller_child_index = right_child_index;
        }

        // 현재 노드가 더 작은 자식보다 우선순위가 낮으면(마감일이 늦으면) 위치 교체
        if (heap[index]->getEndDate() > heap[smaller_child_index]->getEndDate()) {
            std::swap(heap[index], heap[smaller_child_index]);
            index = smaller_child_index; // 인덱스를 자식 위치로 변경하고 계속 진행
        } else {
            // 자식보다 우선순위가 높으면 제자리를 찾은 것이므로 종료
            break;
        }
    }
}


// --- Public Member Functions ---

void PriorityQueue::push(Task* task) {
    // 1. 데이터를 벡터의 맨 끝에 추가합니다.
    heap.push_back(task);
    // 2. 맨 끝 노드부터 시작해서, 제자리를 찾아 위로 올립니다(heapify_up).
    heapify_up(heap.size() - 1);
}

void PriorityQueue::pop() {
    if (empty()) {
        return;
    }

    // 1. 루트 노드(가장 우선순위 높은)와 마지막 노드를 교체합니다.
    std::swap(heap[0], heap.back());
    // 2. 원래 루트였던 마지막 노드를 벡터에서 제거합니다.
    heap.pop_back();

    // 3. 바뀐 루트 노드를 제자리로 찾아 아래로 내립니다(heapify_down).
    if (!empty()) {
        heapify_down(0);
    }
}

Task* PriorityQueue::front() {
    return empty() ? nullptr : heap.front();
}

void PriorityQueue::clear() {
    heap.clear();
}

bool PriorityQueue::empty() const {
    return heap.empty();
}

} // namespace scheduler