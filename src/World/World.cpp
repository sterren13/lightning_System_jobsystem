//
// Created by sterr on 24/04/2023.
//

#include "World/World.h"

World::World() {

}

World::~World() {

}

void World::OnInit() {
    rootGroup.OnInit();
}

void World::OnStart() {
    rootGroup.OnStart();
}

void World::OnUpdate() {
    rootGroup.OnUpdate();
}

void World::OnStop() {
    rootGroup.OnStop();
}

void World::OnDestroy() {
    rootGroup.OnDestroy();
}

void World::PrintGroupTree() {
    std::cout << "========================= Hierarchy of Groups and Members ============================" << std::endl;
    rootGroup.PrintHierarchy();
    std::cout << "======================================================================================" << std::endl;
}
