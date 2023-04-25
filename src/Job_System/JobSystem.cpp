//
// Created by sterr on 25/04/2023.
//

#include "Job_System/JobSystem.h"
#include <iostream>

JobSystem::JobSystem() {
    WorkerCount = std::thread::hardware_concurrency()-1;
    m_WorkerCount = WorkerCount;
    for (size_t i = 0; i < WorkerCount; i++) {
        std::thread worker(&JobSystem::WorkerTask, this, i);
        worker.detach();
    }
    while (!m_Ready) {}
    std::cout << "Job system is ready\n";
}

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n-1) + fibonacci(n-2);
}

void JobSystem::WorkerTask(size_t workerIndex) noexcept {
    m_workerIndex = workerIndex;
    static std::atomic<size_t> workerCount = m_WorkerCount.load();
    static std::mutex PrintMutex;

    PrintMutex.lock();
    std::cout << "Worker " << workerIndex << " is ready\n";
    PrintMutex.unlock();
    workerCount--;
    while (workerCount.load() > 0) {}
    m_Ready = true;

    while (!m_Terminate){
        if (m_JobQueue.size() > 0) {
            std::shared_ptr<Job> job = m_JobQueue.pop();
            if (job) {
                job->Execute();
            }
        }
    }

    PrintMutex.lock();
    std::cout << "Worker " << workerIndex << " is terminated\n";
    PrintMutex.unlock();
    size_t runingWorkers = m_WorkerCount.fetch_sub(1);
    if (runingWorkers == 1) {
        m_Terminated = true;
    }
}

void JobSystem::Terminate() {
    m_Terminate = true;
    while (!m_Terminated) {} // wait until all workers are terminated
    std::cout << "Job system is terminated\n";
}

void JobSystem::Submit(const std::shared_ptr<Job> &job) {
    m_JobQueue.push(job);
}
