#include <iostream>


// group system
#include "Groups/Group.hpp"
#include "Groups/GroupTraits.hpp"
#include "Groups/IMember.hpp"
#include "World/World.h"

// job system
#include "Job_System/JobSystem.h"

class InitGroupTrait : public GroupTraits {
public:
    static constexpr enum GroupParallelJobSystem GroupParallelJobSystem = GroupParallelJobSystem::Disabled;
    static constexpr const size_t GroupId = 150;
    static constexpr const char* GroupName = "InitGroup";
    static constexpr const size_t Before[] = {0};
    static constexpr const size_t After[] = {1502};
};
typedef Group<InitGroupTrait, RootGroup> InitGroup;

class TestGroupTrait1 : public GroupTraits {
public:
    static constexpr enum GroupParallelJobSystem GroupParallelJobSystem = GroupParallelJobSystem::Disabled;
    static constexpr const size_t GroupId = 1501;
    static constexpr const char* GroupName = "TestGroup1";
    static constexpr const size_t Before[] = {150};
    static constexpr const size_t After[] = {0};
};
typedef Group<TestGroupTrait1, RootGroup> TestGroup1;

class TestGroupTrait2 : public GroupTraits {
public:
    static constexpr enum GroupParallelJobSystem GroupParallelJobSystem = GroupParallelJobSystem::Disabled;
    static constexpr const size_t GroupId = 1502;
    static constexpr const size_t Before[] = {1};
    static constexpr const size_t After[] = {2};
    static constexpr const char* GroupName = "TestGroup2";
};
typedef Group<TestGroupTrait2, InitGroup> TestGroup2;

class testMember1 : public IMember<InitGroup> {
public:
    testMember1() {
        MemberId = 1;
        MemberName = "testMember1";
        RegisterMember();
    }

    void OnInit() override {
        std::cout << "testMember1::OnInit" << std::endl;
    }

    void OnStart() override {
        std::cout << "testMember1::OnStart" << std::endl;
    }

    void OnUpdate() override {
        std::cout << "testMember1::OnSystemUpdate" << std::endl;
    }

    void OnStop() override {
        std::cout << "testMember1::OnStop" << std::endl;
    }

    void OnDestroy() override {
        std::cout << "testMember1::OnDestroy" << std::endl;
    }
};

class testMember2 : public IMember<InitGroup> {
public:
    testMember2() {
        MemberId = 2;
        MemberName = "testMember2";
        UpdateBefore.insert(1);
        RegisterMember();
    }

    void OnInit() override {
        std::cout << "testMember2::OnInit" << std::endl;
    }

    void OnStart() override {
        std::cout << "testMember2::OnStart" << std::endl;
    }

    void OnUpdate() override {
        std::cout << "testMember2::OnSystemUpdate" << std::endl;
    }

    void OnStop() override {
        std::cout << "testMember2::OnStop" << std::endl;
    }

    void OnDestroy() override {
        std::cout << "testMember2::OnDestroy" << std::endl;
    }
};

std::atomic_flag m_lock = ATOMIC_FLAG_INIT;

class testJob : public Job {
public:
    testJob(int id) : jobID(id) {}
    int jobID = 0;
    void Execute() override {
        int result = 0;
        for (size_t i = 0; i < 10000+jobID; i++) {
            result += i;
        }
        while (m_lock.test_and_set(std::memory_order_acquire)) {}
        std::cout << "testJob::Execute: " << jobID << ", result: " << result << std::endl;
        m_lock.clear(std::memory_order_release);
    }
};

int main() {
    World world;

    // add members
    testMember1 test1;
    testMember2 test2;

    // add groups
    InitGroup initGroup;
    TestGroup1 testGroup1;
    TestGroup2 testGroup2;

    world.PrintGroupTree();

    JobSystem js;
    for (size_t i = 0; i < 10000; i++) {
        js.Submit(std::make_shared<testJob>(i));
    }
    while (js.IsDone() == false) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    js.Terminate();

    //world.OnInit();
    //world.OnStart();
    //world.OnUpdate();
    //world.OnStop();
    //world.OnDestroy();

    return 0;
}
