//
// Created by sterr on 7/05/2023.
//

#ifndef TESTSYSTEMS_TASKQUEUE_H
#define TESTSYSTEMS_TASKQUEUE_H
#include <vector>
#include <mutex>
#include "TaskHandel.h"

class TaskQueue {
public:
    void push(std::shared_ptr<TaskHandel> task);
    std::shared_ptr<TaskHandel> pop();
    inline size_t size() {return m_taskQueue.size();}

private:
    std::vector<std::shared_ptr<TaskHandel>> m_taskQueue;
    std::mutex taskQueueMutex;
};


#endif //TESTSYSTEMS_TASKQUEUE_H
