//
// Created by sterr on 23/04/2023.
//

#ifndef TESTSYSTEMS_WORKER_MANGER_H
#define TESTSYSTEMS_WORKER_MANGER_H
#include <vector>
#include "Job_Queue.h"

#include "Worker.h"

class Worker_Manger {
public:
    static void Init();
    static void Destroy();

    inline static Worker_Manger* GetInstance() { return Instance;}

    Worker_Manger();

    void Wait();

    inline void ScheduleJob(const std::shared_ptr<Job>& job) { JobQueue.Push(job); }

private:
    static Worker_Manger* Instance;
    const size_t WorkerCount;
    Job_Queue JobQueue;
    std::vector<Worker*> Workers;
    std::condition_variable JobQueueEmpty;
};


#endif //TESTSYSTEMS_WORKER_MANGER_H
