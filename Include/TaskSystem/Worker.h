//
// Created by sterr on 7/05/2023.
//

#ifndef TESTSYSTEMS_WORKER_H
#define TESTSYSTEMS_WORKER_H

#include "TaskHandel.h"
#include "TaskQueue.h"
#include <atomic>
#include <condition_variable>

class Worker {
public:
    Worker(size_t workerIndex);
    std::shared_ptr<TaskHandel> StealTask();
    void Shutdown();
    void WaitToTerminate();
    void Ready();
    void PushTask(const std::shared_ptr<TaskHandel>& task);
    void WakeUp();
    bool IsRunning() { return m_running; }
    bool IsReady() { return m_Ready; }
    bool IsTerminated() { return m_Terminated; }
    size_t GetTaskCount() { return m_TaskQueue.size(); }
    size_t GetWorkerIndex() { return m_workerIndex; }
private:
    void WorkerTask(size_t workerIndex) noexcept;
    void Sleep();
private:
    std::mutex wakeMutex;
    std::condition_variable sleepCondition;
    TaskQueue m_TaskQueue;
    std::atomic_bool m_Ready = false;
    std::atomic_bool m_running = false;
    std::atomic_bool m_Terminate = false;
    std::atomic_bool m_Terminated = false;
    size_t m_workerIndex;
    std::thread m_WorkerThread;
};


#endif //TESTSYSTEMS_WORKER_H
