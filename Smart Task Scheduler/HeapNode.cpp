#include "HeapNode.h" 
// 생성자, task, left, right, parent에 대한 getter & setter

namespace scheduler
{
    HeapNode::HeapNode(scheduler::Task* t) :
        task(t), left_child(nullptr), right_child(nullptr),parent(nullptr){ }
   
    scheduler::Task* HeapNode::getTask() const {
        return task;
    }

    void HeapNode::setTask(scheduler::Task* t) {
        task = t;
    }

    HeapNode* HeapNode::getLeft() const {
        return left_child;
    }

    void HeapNode::setLeft(HeapNode* n) {
        left_child = n;
    }

    HeapNode* HeapNode::getRight() const {
        return right_child;
    }

    void HeapNode::setRight(HeapNode* n) {
        right_child = n;
    }

    HeapNode* HeapNode::getParent() const {
        return parent;
    }

    void HeapNode::setParent(HeapNode* n) {
        parent = n;
    }


}