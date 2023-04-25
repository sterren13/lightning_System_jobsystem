//
// Created by sterr on 23/04/2023.
//

#include "Job_System/Worker_Manger.h"
#include <iostream>
#include "Job_System/Worker.h"

Worker_Manger* Worker_Manger::Instance = nullptr;

void Worker_Manger::Init() {
    if (Instance == nullptr)
        Instance = new Worker_Manger();
}

void Worker_Manger::Destroy() {
    for (auto worker : Instance->Workers) {
        worker->Stop();
    }
    if (Instance != nullptr) {
        delete Instance;
        Instance = nullptr;
    }
}

Worker_Manger::Worker_Manger() : WorkerCount(std::thread::hardware_concurrency()) {
    std::cout << "Creating " << WorkerCount << " workers" << std::endl;
    for (size_t i = 0; i < WorkerCount; i++) {
        Workers.push_back(new Worker(JobQueue));
    }
}

void Worker_Manger::Wait() {
    while(true) {
        if (JobQueue.IsEmpty()) {
            break;
        }
    }
}
