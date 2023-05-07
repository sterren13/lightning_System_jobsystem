//
// Created by sterr on 7/05/2023.
//

#include "TaskSystem/TaskQueue.h"
#include "TaskSystem/TaskHandel.h"

void TaskQueue::push(std::shared_ptr<TaskHandel> task) {
    std::lock_guard<std::mutex> lock(taskQueueMutex);
    m_taskQueue.push_back(task);
}

std::shared_ptr<TaskHandel> TaskQueue::pop() {
    if (m_taskQueue.empty()) return nullptr;
    std::lock_guard<std::mutex> lock(taskQueueMutex);
    for (auto it = m_taskQueue.begin(); it != m_taskQueue.end(); ++it) {
        if ((*it)->isReady()) {
            auto task = *it;
            m_taskQueue.erase(it);
            return task;
        }
    }
    return nullptr;
}