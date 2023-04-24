//
// Created by sterr on 23/04/2023.
//

#ifndef TESTSYSTEMS_WORKER_H
#define TESTSYSTEMS_WORKER_H
#include <thread>
#include <atomic>

class Worker {
public:
    Worker();
    ~Worker();
private:
    std::thread Thread;
    std::atomic<bool> Running;
    void Run();
};


#endif //TESTSYSTEMS_WORKER_H
