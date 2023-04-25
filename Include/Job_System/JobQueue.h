//
// Created by sterr on 25/04/2023.
//

#ifndef TESTSYSTEMS_JOBQUEUE_H
#define TESTSYSTEMS_JOBQUEUE_H
#include <atomic>
#include <memory>
#include <queue>
#include "Job.h"

class JobQueue {
public:
    JobQueue();

    ~JobQueue();

    inline size_t size() const { return m_queue.size(); }

    void push(const std::shared_ptr<Job>& job);

    std::shared_ptr<Job> pop();

private:
    std::atomic_flag m_lock = ATOMIC_FLAG_INIT; // for lock-free push/pop operations
    std::queue<std::shared_ptr<Job>> m_queue; // queue of jobs
};


#endif //TESTSYSTEMS_JOBQUEUE_H
