#include "Priority_queue.h"
#include <queue>


namespace scheduler {
    HeapNode* PriorityQueue::find_insertion_point() {
        if (!root) return nullptr;

        std::queue<HeapNode*> q;
        q.push(root);

        while (!q.empty()) {
            HeapNode* current = q.front();
            q.pop();

            if (current->getLeft() == nullptr || current->getRight() == nullptr) {
                return current;
            }

            if (current->getLeft()) q.push(current->getLeft());
            if (current->getRight()) q.push(current->getRight());
        }
        return nullptr;
    }

    HeapNode* PriorityQueue::find_last_node() {
        if (!root) return nullptr;

        // 루트부터 BFS를 수행.
        std::queue<HeapNode*> q;
        q.push(root);
        HeapNode* last = root;

        while (!q.empty()) {
            last = q.front();
            q.pop();

            if (last->getLeft()) q.push(last->getLeft());
            if (last->getRight()) q.push(last->getRight());
        }
        return last;
    }
 
    void PriorityQueue::heapify_up(HeapNode* node) {
        if (!node || !node->getParent()) return;

        HeapNode* current = node;
        HeapNode* parent = current->getParent();

        while (parent && current->getTask()->getEndDate() < parent->getTask()->getEndDate()) {

            // Task 데이터(포인터)만 교환
            Task* temp = current->getTask();
            current->setTask(parent->getTask());
            parent->setTask(temp);

            // 다음 레벨
            current = parent;
            parent = current->getParent();
        }
    }

    //  힙 속성 복원
    void PriorityQueue::heapify_down(HeapNode* node) {
        if (!node) return;

        HeapNode* current = node;
        HeapNode* smaller_child;

        while (true) {
            HeapNode* left = current->getLeft();
            HeapNode* right = current->getRight();
            smaller_child = nullptr;

            if (left && right) {
                if (left->getTask()->getEndDate() < right->getTask()->getEndDate()) {
                    smaller_child = left;
                }
                else {
                    smaller_child = right;
                }
            }
            else if (left) {
                smaller_child = left;
            }
            else if (right) {
                smaller_child = right;
            }

            if (smaller_child && smaller_child->getTask()->getEndDate() < current->getTask()->getEndDate()) {

                // Task 데이터(포인터) 교환
                scheduler::Task* temp = current->getTask();
                current->setTask(smaller_child->getTask());
                smaller_child->setTask(temp);

                // 다음 레벨로 이동
                current = smaller_child;
            }
            else {
                break;
            }
        }
    }

    void PriorityQueue::push(scheduler::Task* task) {
        HeapNode* newNode = new HeapNode(task);

        if (size == 0) {
            root = newNode;
            size = 1;
            return;
        }

        // 1. O(N)
        HeapNode* parent = find_insertion_point();

        // 2. 새 노드 연결
        newNode->setParent(parent);
        if (parent->getLeft() == nullptr) {
            parent->setLeft(newNode);
        }
        else {
            parent->setRight(newNode);
        }
        size++;

        // 3. O(logN): 힙 속성 복원
        heapify_up(newNode);
    }

    void PriorityQueue::pop() {
        if (size == 0) return;

        if (size == 1) {
            delete root;
            root = nullptr;
            size = 0;
            return;
        }

        // 1. O(N)
        HeapNode* lastNode = find_last_node();

        root->setTask(lastNode->getTask());

        HeapNode* parent = lastNode->getParent(); // 연결 후 해제
        if (parent->getLeft() == lastNode) {
            parent->setLeft(nullptr);
        }
        else {
            parent->setRight(nullptr);
        }

        delete lastNode;
        size--;

        if (root) {
            heapify_down(root);
        }
    }

    scheduler::Task* PriorityQueue::front() {
        return (size == 0) ? nullptr : root->getTask();
    }

    bool PriorityQueue::empty() const {
        return size == 0;
    }

-    void PriorityQueue::clear() {
        if (!root) return;

        // BFS를 사용하여 모든 노드를 순회하며 메모리 해제
        std::queue<HeapNode*> q;
        q.push(root);

        while (!q.empty()) {
            HeapNode* current = q.front();
            q.pop();

            if (current->getLeft()) q.push(current->getLeft());
            if (current->getRight()) q.push(current->getRight());

            delete current;
        }

        root = nullptr;
        size = 0;
    }

} 