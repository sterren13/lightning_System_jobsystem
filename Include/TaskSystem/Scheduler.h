//
// Created by sterr on 7/05/2023.
//

#ifndef TESTSYSTEMS_SCHEDULER_H
#define TESTSYSTEMS_SCHEDULER_H
#include <memory>
#include <vector>
#include <atomic>
#include <mutex>
#include "TaskHandel.h"

class Worker;
class Scheduler {
public:
    static Scheduler& GetInstance();
    void Init();
    void Shutdown();
    void PrintStats();
    bool WorkersAreDone();

    std::shared_ptr<TaskHandel> Schedule(const std::shared_ptr<ITask>& task);
    std::shared_ptr<TaskHandel> Schedule(const std::shared_ptr<ITask>& task, const std::shared_ptr<TaskHandel>& dependant);
    std::shared_ptr<TaskHandel> Schedule_OnWorker(const std::shared_ptr<ITask>& task, size_t workerIndex);
    std::shared_ptr<TaskHandel> Schedule_OnWorker(const std::shared_ptr<ITask>& task, size_t workerIndex, const std::shared_ptr<TaskHandel>& dependant);
private:
    Scheduler() = default;
    ~Scheduler() = default;
    friend Worker;
    std::shared_ptr<TaskHandel> StealTask();
    void WakeUpAllWorkers();

private:
    std::vector<std::shared_ptr<Worker>> m_Workers;
    std::mutex WorkerVectorMutex;
    size_t WorkerCount;
};

#endif //TESTSYSTEMS_SCHEDULER_H
