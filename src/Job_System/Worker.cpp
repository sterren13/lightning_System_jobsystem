//
// Created by sterr on 23/04/2023.
//

#include <thread>
#include <iostream>
#include "Job_System/Worker_Manger.h"
#include "Job_System/Worker.h"


Worker_Manger* Worker_Manger::Instance = nullptr;

void Worker_Manger::Init() {
    if (Instance == nullptr)
        Instance = new Worker_Manger();
}

void Worker_Manger::Destroy() {
    if (Instance != nullptr) {
        delete Instance;
        Instance = nullptr;
    }
}

Worker_Manger::Worker_Manger() : WorkerCount(std::thread::hardware_concurrency()) {
    std::cout << "Creating " << WorkerCount << " workers" << std::endl;
    for (size_t i = 0; i < WorkerCount; i++) {
        Workers.push_back(new Worker());
    }
}

Worker::Worker() {

}

Worker::~Worker() {

}

void Worker::Run() {

}
