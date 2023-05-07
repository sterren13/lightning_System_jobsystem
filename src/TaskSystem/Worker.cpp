//
// Created by sterr on 7/05/2023.
//

#include "TaskSystem/Worker.h"
#include "TaskSystem/Scheduler.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

Worker::Worker(size_t workerIndex) {
    m_WorkerThread = std::thread(&Worker::WorkerTask, this, workerIndex);
#ifdef _WIN32
    HANDLE nativeHandle = m_WorkerThread.native_handle();
    SetThreadPriority(nativeHandle, THREAD_PRIORITY_HIGHEST);
    DWORD_PTR affinityMask = 1 << workerIndex;
    SetThreadAffinityMask(nativeHandle, affinityMask);
#else
    pthread_t nativeHandle = threads[i].native_handle();
        struct sched_param params;
        params.sched_priority = sched_get_priority_max(SCHED_FIFO);
        pthread_setschedparam(nativeHandle, SCHED_FIFO, &params);
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(workerIndex, &cpuset);
        pthread_setaffinity_np(nativeHandle, sizeof(cpu_set_t), &cpuset);
#endif
    m_WorkerThread.detach();
}

std::shared_ptr<TaskHandel> Worker::StealTask() {
    return m_TaskQueue.pop();
}

void Worker::Shutdown() {
    m_Terminate = true;
    WakeUp();
}

void Worker::Ready() {
    while (!m_Ready);
    std::cout << "Worker " << m_workerIndex << " is ready" << std::endl;
}

void Worker::WaitToTerminate() {
    while (m_TaskQueue.size() > 0);
    while (!m_Terminated);
    std::cout << "Worker " << m_workerIndex << " is terminated" << std::endl;
}

void Worker::WorkerTask(size_t workerIndex) noexcept {
    m_workerIndex = workerIndex;
    m_Ready = true;
    m_running = true;
    while(!m_Terminate){
        if (m_TaskQueue.size() > 0){
            auto task = m_TaskQueue.pop();
            if (task) {
                task->Run();
                Scheduler::GetInstance().WakeUpAllWorkers();
            }
            else Sleep();
        }
        else {
            auto task = Scheduler::GetInstance().StealTask();
            if (task) {
                task->Run();
                Scheduler::GetInstance().WakeUpAllWorkers();
            }
            else Sleep();
        }
    }
    m_Terminated = true;
}

void Worker::PushTask(const std::shared_ptr<TaskHandel>& task) {
    m_TaskQueue.push(task);
    task->m_workerIndex = m_workerIndex;
    WakeUp();
}

void Worker::WakeUp() {
    sleepCondition.notify_one();
}

void Worker::Sleep() {
    m_running = false;
    std::unique_lock<std::mutex> lock(wakeMutex);
    sleepCondition.wait(lock);
    m_running = true;
}
