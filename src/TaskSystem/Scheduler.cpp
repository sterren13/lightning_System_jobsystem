//
// Created by sterr on 7/05/2023.
//

#include "TaskSystem/Scheduler.h"
#include "TaskSystem/Worker.h"
#include <thread>
#include <iostream>

Scheduler &Scheduler::GetInstance() {
    static Scheduler instance;
    return instance;
}

void Scheduler::Init() {
    std::lock_guard<std::mutex> lock(WorkerVectorMutex);
    WorkerCount = std::thread::hardware_concurrency() -1;
    //WorkerCount = 2;
    for (size_t i = 0; i < WorkerCount; i++)
        m_Workers.push_back(std::make_shared<Worker>(i));
    std::cout << "Scheduler created with " << WorkerCount << " workers" << std::endl;
    for (auto worker : m_Workers)
        worker->Ready();
}

void Scheduler::Shutdown() {
    std::lock_guard<std::mutex> lock(WorkerVectorMutex);
    for (auto worker : m_Workers)
        worker->Shutdown();
    for (auto worker : m_Workers)
        worker->WaitToTerminate();
    m_Workers.clear();
}

std::shared_ptr<TaskHandel> Scheduler::Schedule(const std::shared_ptr<ITask>& task) {
    auto TaskH = std::make_shared<TaskHandel>(task);
    std::lock_guard<std::mutex> lock(WorkerVectorMutex);
    std::shared_ptr<Worker> worker = m_Workers[0];
    for (auto w : m_Workers)
        if (w->GetTaskCount() < worker->GetTaskCount()) {
            worker = w;
        }
    worker->PushTask(TaskH);
    return TaskH;
}

std::shared_ptr<TaskHandel> Scheduler::Schedule(const std::shared_ptr<ITask>& task, const std::shared_ptr<TaskHandel>& dependant) {
    auto TaskH = std::make_shared<TaskHandel>(task, dependant);
    std::lock_guard<std::mutex> lock(WorkerVectorMutex);
    std::shared_ptr<Worker> worker = m_Workers[dependant->GetWorkerIndex()];
    worker->PushTask(TaskH);
    return TaskH;
}

std::shared_ptr<TaskHandel> Scheduler::Schedule_OnWorker(const std::shared_ptr<ITask> &task, size_t workerIndex) {
    auto TaskH = std::make_shared<TaskHandel>(task);
    std::lock_guard<std::mutex> lock(WorkerVectorMutex);
    std::shared_ptr<Worker> worker = m_Workers[workerIndex];
    worker->PushTask(TaskH);
    return TaskH;
}

std::shared_ptr<TaskHandel> Scheduler::Schedule_OnWorker(const std::shared_ptr<ITask> &task, size_t workerIndex,
                                                         const std::shared_ptr<TaskHandel> &dependant) {
    auto TaskH = std::make_shared<TaskHandel>(task, dependant);
    std::lock_guard<std::mutex> lock(WorkerVectorMutex);
    std::shared_ptr<Worker> worker = m_Workers[workerIndex];
    worker->PushTask(TaskH);
    return TaskH;
}

std::shared_ptr<TaskHandel> Scheduler::StealTask() {
    std::lock_guard<std::mutex> lock(WorkerVectorMutex);
    for (auto worker : m_Workers) {
        auto task = worker->StealTask();
        if (task != nullptr)
            return task;
    }
    return nullptr;
}

void Scheduler::PrintStats() {
    std::lock_guard<std::mutex> lock(WorkerVectorMutex);
    for (auto worker : m_Workers)
        std::cout << "Worker " << worker->GetWorkerIndex() << " has " << worker->GetTaskCount() << " task, "
                  << "state: " << worker->IsRunning() << "\n";
}

bool Scheduler::WorkersAreDone() {
    std::lock_guard<std::mutex> lock(WorkerVectorMutex);
    for (auto worker : m_Workers)
        if (worker->IsRunning())
            return false;
    return true;
}

void Scheduler::WakeUpAllWorkers() {
    std::lock_guard<std::mutex> lock(WorkerVectorMutex);
    for (auto worker : m_Workers)
        worker->WakeUp();
}
