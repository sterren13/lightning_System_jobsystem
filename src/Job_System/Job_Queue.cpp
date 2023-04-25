//
// Created by sterr on 24/04/2023.
//

#include "Job_System/Job_Queue.h"

void Job_Queue::Push(const std::shared_ptr<Job>& job) {
    Queue_Mutex.lock();
    Queue.push(job);
    Queue_Mutex.unlock();
}

std::shared_ptr<Job> Job_Queue::Pop() {
    Queue_Mutex.lock();
    if (Queue.empty()) {
        Queue_Mutex.unlock();
        return nullptr;
    }
    auto job = Queue.front();
    Queue.pop();
    Queue_Mutex.unlock();
    return job;
}