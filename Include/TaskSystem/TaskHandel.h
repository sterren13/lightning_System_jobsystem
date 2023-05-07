//
// Created by sterr on 7/05/2023.
//

#ifndef TESTSYSTEMS_TASKHANDEL_H
#define TESTSYSTEMS_TASKHANDEL_H
#include <atomic>
#include <memory>

struct ITask;
class TaskHandel {
public:
    TaskHandel(std::shared_ptr<ITask> task, std::shared_ptr<TaskHandel> dependant);
    TaskHandel(std::shared_ptr<ITask> task);

    bool isDone() const;
    void wait();
    size_t GetWorkerIndex() const {return m_workerIndex;}

private:
    friend class Worker;
    friend class TaskQueue;
    bool isReady() const;
    void Run();

private:
    std::shared_ptr<ITask> m_task;
    std::shared_ptr<TaskHandel> m_parent;
    std::atomic_size_t m_childCount;
    std::atomic_bool m_done = false;
    size_t m_workerIndex = 0;
};

#endif //TESTSYSTEMS_TASKHANDEL_H
