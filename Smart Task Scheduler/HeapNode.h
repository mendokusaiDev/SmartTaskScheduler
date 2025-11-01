#pragma once
#include "Task.h"

namespace scheduler
{
    class HeapNode
    {
    private:

        Task* task;
        HeapNode* left_child;
        HeapNode* right_child;
        HeapNode* parent;


    public:

        HeapNode(Task* t);
        Task* getTask() const;
        void setTask(Task* tast);

        HeapNode* getLeft() const;
        void setLeft(HeapNode* hn);

        HeapNode* getRight() const;
        void setRight(HeapNode* hn);

        HeapNode* getParent() const;
        void setParent(HeapNode* hn);
    };
};