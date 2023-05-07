//
// Created by sterr on 23/04/2023.
//

#include "Groups/RootGroup.h"
#include "TaskSystem/Scheduler.h"

void RootGroup::OnInit()  {
    for (auto& member : GetMembers()) {
        member->OnInit();
    }
}

void RootGroup::OnStart() {
    for (auto& member : GetMembers()) {
        if (member->IsEnabled) member->OnStart();
    }
}

void RootGroup::OnUpdate() {
    for (auto& member : GetMembers()) {
        if (member->IsEnabled) member->OnUpdate();
    }
    while (!Scheduler::GetInstance().WorkersAreDone()){}
}

void RootGroup::OnStop()  {
    for (auto& member : GetMembers()) {
        if (member->IsEnabled) member->OnStop();
    }
}

void RootGroup::OnDestroy() {
    for (auto& member : GetMembers()) {
        if (member->IsEnabled) member->OnDestroy();
    }
}

void RootGroup::PrintHierarchy() {
    std::cout << "RootGroup: " << std::endl;
    for (auto& member : GetMembers()) {
        member->PrintHierarchy(1);
    }
}

void RootGroup::AddMember(BaseMember *member) {
    std::vector<BaseMember*>& vec = GetMembers();
    vec.push_back(member);
    std::sort(vec.begin(), vec.end(), SortByUpdateOrder);
}

void RootGroup::RemoveMember(BaseMember *member) {
    auto& members = GetMembers();
    members.erase(std::remove(members.begin(), members.end(), member), members.end());
}

std::vector<BaseMember *> &RootGroup::GetMembers() {
    static std::vector<BaseMember*> members;
    return members;
}

bool RootGroup::SortByUpdateOrder(BaseMember *LM, BaseMember *RM) {
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