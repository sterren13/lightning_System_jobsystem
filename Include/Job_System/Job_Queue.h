//
// Created by sterr on 24/04/2023.
//

#ifndef TESTSYSTEMS_JOB_QUEUE_H
#define TESTSYSTEMS_JOB_QUEUE_H
#include <vector>
#include <mutex>
#include <memory>
#include <queue>
#include "Job.h"

class Job_Queue {
public:
    void Push(const std::shared_ptr<Job>& job);
    std::shared_ptr<Job> Pop();
    [[nodiscard]] inline bool IsEmpty() const { return Queue.empty();}
    [[nodiscard]] inline size_t Size() const { return Queue.size();}
public:
    std::mutex Queue_Mutex;
    std::queue<std::shared_ptr<Job>> Queue;
};


#endif //TESTSYSTEMS_JOB_QUEUE_H
