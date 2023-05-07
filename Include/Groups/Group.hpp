//
// Created by sterr on 23/04/2023.
//

#ifndef TESTSYSTEMS_GROUP_HPP
#define TESTSYSTEMS_GROUP_HPP
#include <type_traits>
#include <vector>
#include <iostream>

#include "GroupTraits.hpp"
#include "BaseMember.hpp"
#include "RootGroup.h"
#include "TaskSystem/Scheduler.h"

template<class Traits, class ParentGroup, GroupParallelJobSystem ParallelJobSystem = Traits::GroupParallelJobSystem>
class Group : public Traits, public BaseMember {
    static_assert(std::is_base_of<GroupTraits, Traits>::value, "Traits must be derived from BaseGroup");
    static_assert(std::is_base_of<BaseMember, ParentGroup>::value || std::is_same<RootGroup, ParentGroup>::value, "ParentGroup must be derived from BaseMember");
    Group() = default;
    ~Group() = default;

    void OnInit() final {
        // TODO log error not implemented
    }

    void OnStart() final {
        // TODO log error not implemented
    }

    void OnUpdate() final {
        // TODO log error not implemented
    }

    void OnStop() final {
        // TODO log error not implemented
    }

    void OnDestroy() final {
        // TODO log error not implemented
    }

    void PrintHierarchy(size_t interval) final {
        // TODO log error not implemented
    }
};

/*
 * Group Parallel Job System disabled
 */
template<class Traits, class ParentGroup>
class Group<Traits, ParentGroup, GroupParallelJobSystem::Disabled> : public Traits, public BaseMember {
public:
    static_assert(std::is_base_of<GroupTraits, Traits>::value, "Traits must be derived from BaseGroup");
    static_assert(std::is_base_of<BaseMember, ParentGroup>::value || std::is_same<RootGroup, ParentGroup>::value, "ParentGroup must be derived from BaseMember");
    Group() {
        // insert update order
        for (size_t i = 0; i < sizeof(Traits::Before) / sizeof(size_t); i++)
            if (Traits::Before[i] != 0) UpdateBefore.insert(Traits::Before[i]);
        for (size_t i = 0; i < sizeof(Traits::After) / sizeof(size_t); i++)
            if (Traits::After[i] != 0) UpdateAfter.insert(Traits::After[i]);
        MemberName = Traits::GroupName;
        MemberId = Traits::GroupId;
        ParentGroup::AddMember(this);
    }
    ~Group() {
        ParentGroup::RemoveMember(this);
    }

    void PrintHierarchy(size_t interval) final {
        for (size_t i = 0; i < interval; i++) std::cout << "  ";
        std::cout << "| Group: " << Traits::GroupName << ", ID: " << MemberId << ":" << std::endl;
        for (auto &member : GetMembers()) {
            if (member->IsEnabled) member->PrintHierarchy(interval + 1);
        }
    }

    void OnInit() final {
        for (auto &member : GetMembers()) {
            member->OnInit();
        }
    }

    void OnStart() final {
        for (auto &member : GetMembers()) {
            if (member->IsEnabled) member->OnStart();
        }
    }

    void OnUpdate() final {
        for (auto &member : GetMembers())
            if (member->IsEnabled) member->OnUpdate();
    }

    void OnStop() final {
        for (auto &member : GetMembers()) {
            if (member->IsEnabled) member->OnStop();
        }
    }

    void OnDestroy() final {
        for (auto &member : GetMembers()) {
            if (member->IsEnabled) member->OnDestroy();
        }
    }

    static void AddMember(BaseMember* member) {
        std::vector<BaseMember*>& vec = GetMembers();
        vec.push_back(member);
        std::sort(vec.begin(), vec.end(), SortByUpdateOrder);
    }
    static void RemoveMember(BaseMember* member) {
        auto& members = GetMembers();
        members.erase(std::remove(members.begin(), members.end(), member), members.end());
    }
private:
    static std::vector<BaseMember*>& GetMembers() {
        static std::vector<BaseMember*> members;
        return members;
    }

    static bool SortByUpdateOrder(BaseMember* LM, BaseMember* RM) {
        // First, check if LM depends on RM
        bool LM_Before_RM = LM->UpdateBefore.contains(RM->MemberId);
        bool LM_After_RM = LM->UpdateAfter.contains(RM->MemberId);
        bool RM_Before_LM = RM->UpdateBefore.contains(LM->MemberId);
        bool RM_After_LM = RM->UpdateAfter.contains(LM->MemberId);

        if ((LM_Before_RM && !LM_After_RM) || (!RM_Before_LM && RM_After_LM)) return true; // LM must be updated before RM
        if ((!LM_Before_RM && LM_After_RM) || (RM_Before_LM && !RM_After_LM)) return false; // LM must be updated after RM

        // when LM and RM are independent, sort by MemberId
        return LM->MemberId < RM->MemberId;
    }
};

/*
 * Group Parallel Job System enabled
 */
template<class Traits, class ParentGroup>
class Group<Traits, ParentGroup, GroupParallelJobSystem::Enabled> : public Traits, public BaseMember {
public:
    static_assert(std::is_base_of<GroupTraits, Traits>::value, "Traits must be derived from BaseGroup");
    static_assert(std::is_base_of<BaseMember, ParentGroup>::value || std::is_same<RootGroup, ParentGroup>::value, "ParentGroup must be derived from BaseMember");
    Group() {
        // insert update order
        for (size_t i = 0; i < sizeof(Traits::Before) / sizeof(size_t); i++)
            if (Traits::Before[i] != 0) UpdateBefore.insert(Traits::Before[i]);
        for (size_t i = 0; i < sizeof(Traits::After) / sizeof(size_t); i++)
            if (Traits::Before[i] != 0) UpdateAfter.insert(Traits::After[i]);
        MemberName = Traits::GroupName;
        MemberId = Traits::GroupId;
        ParentGroup::AddMember(this);
    }
    ~Group() {
        ParentGroup::RemoveMember(this);
    }

    void PrintHierarchy(size_t interval) final {
        for (size_t i = 0; i < interval; i++) std::cout << "  ";
        std::cout << "| Group: " << Traits::GroupName << ", ID: " << MemberId << ":" << std::endl;
        for (auto &member : GetMembers()) {
            if (member->IsEnabled) member->PrintHierarchy(interval + 1);
        }
    }

    void OnInit() final {
        for (auto &member : GetMembers()) {
            member->OnInit();
        }
    }

    void OnStart() final {
        for (auto &member : GetMembers()) {
            if (member->IsEnabled) member->OnStart();
        }
    }

    void OnUpdate() final {
        for (auto &member : GetMembers()) {
            if (member->IsEnabled) member->OnUpdate();
        }
        while (!Scheduler::GetInstance().WorkersAreDone()) {
            std::this_thread::yield();
        }
    }

    void OnStop() final {
        for (auto &member : GetMembers()) {
            if (member->IsEnabled) member->OnStop();
        }
    }

    void OnDestroy() final {
        for (auto &member : GetMembers()) {
            if (member->IsEnabled) member->OnDestroy();
        }
    }

    static void AddMember(BaseMember* member) {
        GetMembers().push_back(member);
        std::sort(GetMembers().begin(), GetMembers().end(), SortByUpdateOrder);
    }
    static void RemoveMember(BaseMember* member) {
        auto& members = GetMembers();
        members.erase(std::remove(members.begin(), members.end(), member), members.end());
    }
private:
    static std::vector<BaseMember*>& GetMembers() {
        static std::vector<BaseMember*> members;
        return members;
    }

    static bool SortByUpdateOrder(BaseMember* LM, BaseMember* RM) {
        // First, check if LM depends on RM
        bool LM_Before_RM = LM->UpdateBefore.contains(RM->MemberId);
        bool LM_After_RM = LM->UpdateAfter.contains(RM->MemberId);
        bool RM_Before_LM = RM->UpdateBefore.contains(LM->MemberId);
        bool RM_After_LM = RM->UpdateAfter.contains(LM->MemberId);

        if ((LM_Before_RM && !LM_After_RM) || (!RM_Before_LM && RM_After_LM)) return true; // LM must be updated before RM
        if ((!LM_Before_RM && LM_After_RM) || (RM_Before_LM && !RM_After_LM)) return false; // LM must be updated after RM

        // when LM and RM are independent, sort by MemberId
        return LM->MemberId < RM->MemberId;
    }
};

#endif //TESTSYSTEMS_GROUP_HPP
