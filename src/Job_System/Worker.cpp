//
// Created by sterr on 23/04/2023.
//

#include <thread>
#include <iostream>
#include "Job_System/Worker_Manger.h"
#include "Job_System/Worker.h"

Worker::Worker(Job_Queue &jobQueue) : GlobalJobQueue(jobQueue) {
    Running = true;
    Thread = std::thread(&Worker::Run, this);
}

Worker::~Worker() {
    std::cout << "Destory worker\n";
    Running = false;
    Thread.join();
}

void Worker::Run() {
    while (Running) {
        if (GlobalJobQueue.IsEmpty()) {
            continue;
        }
        std::shared_ptr<Job> job = GlobalJobQueue.Pop();
        if (job) {
            job->Execute();
        }
    }
    /*
    while (Running) {
        std::unique_lock<std::mutex> lock(GlobalJobQueueMutex);
        GlobalJobQueueCondition.wait(lock, [this] { return !GlobalJobQueue.IsEmpty() || !Running; });
        if (!Running) {
            return;
        }
        std::shared_ptr<Job> job = GlobalJobQueue.Pop();
        if (job) {
            job->Execute();
        }
    }
     */
}
