//
// Created by sterr on 23/04/2023.
//

#ifndef TESTSYSTEMS_WORKER_H
#define TESTSYSTEMS_WORKER_H
#include <thread>
#include <atomic>
#include "Job_Queue.h"

class Worker {
public:
    Worker(Job_Queue& jobQueue);
    ~Worker();
    void Join() { Thread.join(); }
    void Stop() { Running = false; }
private:
    std::thread Thread;
    std::atomic_bool Running;
    Job_Queue& GlobalJobQueue;
    void Run();
};


#endif //TESTSYSTEMS_WORKER_H
