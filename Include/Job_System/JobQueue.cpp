//
// Created by sterr on 25/04/2023.
//

#include "JobQueue.h"

JobQueue::JobQueue() {

}

JobQueue::~JobQueue() {

}

void JobQueue::push(const std::shared_ptr<Job> &job) {
    while (m_lock.test_and_set(std::memory_order_acquire)) {}
    m_queue.push(job);
    m_lock.clear(std::memory_order_release);
}

std::shared_ptr<Job> JobQueue::pop() {
    while (m_lock.test_and_set(std::memory_order_acquire)) {}
    if (!m_queue.empty()) {
        auto job = m_queue.front();
        m_queue.pop();
        m_lock.clear(std::memory_order_release);
        return job;
    }
    m_lock.clear(std::memory_order_release);
    return nullptr;
}
