//
// Created by sterr on 7/05/2023.
//

#include "TaskSystem/TaskHandel.h"
#include "TaskSystem/ITask.h"

TaskHandel::TaskHandel(std::shared_ptr<ITask> task, std::shared_ptr<TaskHandel> dependant)
        : m_task(std::move(task)), m_parent(std::move(dependant)) {
    m_childCount = 0;
    m_parent->m_childCount++;
}

TaskHandel::TaskHandel(std::shared_ptr<ITask> task) : m_task(std::move(task)) {
    m_childCount = 0;
    m_parent = nullptr;
}

bool TaskHandel::isDone() const {
    return m_done;
}

void TaskHandel::wait() {
    while (!isDone());
}

bool TaskHandel::isReady() const {
    return (m_parent) ? m_parent->isDone() : true;
}

void TaskHandel::Run() {
    m_task->Execute();
    m_done = true;
    if (m_parent)
        m_parent->m_childCount--;
}
