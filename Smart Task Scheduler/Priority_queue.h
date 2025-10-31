#pragma once
#include "Calender.h"
#include "HeapNode.h"


namespace scheduler {

class PriorityQueue {
private:
    HeapNode* root = nullptr;
    int size = 0;
    void heapify_up(HeapNode * hn);   
    void heapify_down(HeapNode * hn); 

    HeapNode* find_insertion_point(); // 다음 노드가 들어갈 부모 노드 (삽입 시)
    HeapNode* find_last_node();       // 가장 오른쪽, 아래에 있는 노드 (제거 시)

public:
    void clear();
    
    void push(Task* task);  
    void pop();
  
    Task* front();
    bool empty() const;
};

}