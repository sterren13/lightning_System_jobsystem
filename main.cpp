#include <iostream>


// group system
#include "Groups/Group.hpp"
#include "Groups/GroupTraits.hpp"
#include "World/World.h"
#include "TaskSystem/Scheduler.h"
#include "World/ISystem.h"
#include "TaskSystem/ITask.h"
#include <windows.h>

class InitGroupTrait : public GroupTraits {
public:
    static constexpr enum GroupParallelJobSystem GroupParallelJobSystem = GroupParallelJobSystem::Disabled;
    static constexpr const size_t GroupId = 150;
    static constexpr const char* GroupName = "InitGroup";
    static constexpr const size_t Before[] = {1501};
    static constexpr const size_t After[] = {0};
};
typedef Group<InitGroupTrait, RootGroup> InitGroup;

class TestGroupTrait1 : public GroupTraits {
public:
    static constexpr enum GroupParallelJobSystem GroupParallelJobSystem = GroupParallelJobSystem::Enabled;
    static constexpr const size_t GroupId = 1501;
    static constexpr const char* GroupName = "TestGroup1";
    static constexpr const size_t Before[] = {1502};
    static constexpr const size_t After[] = {0};
};
typedef Group<TestGroupTrait1, RootGroup> TestGroup1;

class TestGroupTrait2 : public GroupTraits {
public:
    static constexpr enum GroupParallelJobSystem GroupParallelJobSystem = GroupParallelJobSystem::Enabled;
    static constexpr const size_t GroupId = 1502;
    static constexpr const size_t Before[] = {0};
    static constexpr const size_t After[] = {0};
    static constexpr const char* GroupName = "TestGroup2";
};
typedef Group<TestGroupTrait2, RootGroup> TestGroup2;

int num = 0;

class testSystem1 : public ISystem<TestGroup1> {
public:
    testSystem1() {
        MemberId = 1;
        MemberName = "testMember1";
        RegisterMember();
    }

    void OnInit() override {
        task = std::make_shared<testTask>(&num);
    }

    void OnStart() override {
    }

    struct testTask : public ITask {
        testTask(int* num) : m_num(num) {}

        int* m_num;
        void Execute() override {
            std::cout << "testTask::Execute" << std::endl;
            *m_num = 10;
            Sleep(5000);
        }
    };

    void OnUpdate() override {
        std::cout << "testMember1::OnSystemUpdate" << std::endl;
        Scheduler::GetInstance().Schedule(task);
    }

    void OnStop() override {
    }

    void OnDestroy() override {
    }
private:
    std::shared_ptr<testTask> task;
};

class testSystem2 : public ISystem<TestGroup2> {
public:
    testSystem2() {
        MemberId = 2;
        MemberName = "testMember2";
        UpdateBefore.insert(1);
        RegisterMember();
    }

    struct testTask : public ITask {
        testTask(int* num) : m_num(num) {}

        int* m_num;
        void Execute() override {
            std::cout << "testTask::Execute" << std::endl;
            *m_num = 20;
            Sleep(5000);
        }
    };

    void OnInit() override {
        task = std::make_shared<testTask>(&num);
    }

    void OnStart() override {
    }

    void OnUpdate() override {
        std::cout << "testMember2::OnSystemUpdate" << std::endl;
        std::cout << "Num: " << num << std::endl;
        auto handel = Scheduler::GetInstance().Schedule(task);
        handel->wait();
        std::cout << "Num: " << num << std::endl;
    }

    void OnStop() override {
    }

    void OnDestroy() override {
    }

private:
    std::shared_ptr<testTask> task;
};

int main() {
    Scheduler& scheduler = Scheduler::GetInstance();
    scheduler.Init();
    World world;

    // add members
    world.CreateSystem<testSystem1>();
    world.CreateSystem<testSystem2>();

    // add groups
    InitGroup initGroup;
    TestGroup1 testGroup1;
    TestGroup2 testGroup2;

    world.PrintGroupTree();

    world.OnInit();
    world.OnStart();
    world.OnUpdate();
    world.OnStop();

    world.OnDestroy();
    scheduler.Shutdown();
    return 0;
}
