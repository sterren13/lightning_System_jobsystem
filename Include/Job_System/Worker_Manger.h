//
// Created by sterr on 23/04/2023.
//

#ifndef TESTSYSTEMS_WORKER_MANGER_H
#define TESTSYSTEMS_WORKER_MANGER_H
#include <vector>
#include <queue>

#include "Worker.h"

class Worker_Manger {
public:
    static void Init();
    static void Destroy();

    Worker_Manger();

private:
    static Worker_Manger* Instance;
    const size_t WorkerCount;
    std::vector<Worker*> Workers;
    std::queue<Worker*> FreeWorkers;
};


#endif //TESTSYSTEMS_WORKER_MANGER_H
