//
// Created by sterr on 25/04/2023.
//

#ifndef TESTSYSTEMS_JOBSYSTEM_H
#define TESTSYSTEMS_JOBSYSTEM_H
#include <thread>
#include <atomic>
#include <mutex>

class JobSystem {
public:
    JobSystem();
    void Terminate();
    void WorkerTask(size_t workerIndex) noexcept; // worker task to be run by each worker thread

private:
    static inline thread_local size_t m_workerIndex = 0; // worker index of thread
    static inline std::atomic<size_t> m_WorkerCount = 0; // number of workers wil count down until 0 when all workers are ready
    static inline std::atomic<bool> m_Ready = false; // all workers are ready
    static inline std::atomic<bool> m_Terminate = false; // worker must terminate
    static inline std::atomic<bool> m_Terminated = false; // all workers have terminated
    size_t WorkerCount; // number of workers to create
};


#endif //TESTSYSTEMS_JOBSYSTEM_H
