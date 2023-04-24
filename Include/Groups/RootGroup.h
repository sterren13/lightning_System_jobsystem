//
// Created by sterr on 23/04/2023.
//

#ifndef TESTSYSTEMS_ROOTGROUP_H
#define TESTSYSTEMS_ROOTGROUP_H
#include <vector>
#include <algorithm>
#include <iostream>

#include "BaseMember.hpp"

class RootGroup {
public:
    void OnInit();

    void OnStart();

    void OnUpdate();

    void OnStop();

    void OnDestroy();

    void PrintHierarchy();

    static void AddMember(BaseMember* member);

    static void RemoveMember(BaseMember* member);

private:
    static std::vector<BaseMember*>& GetMembers();

    static bool SortByUpdateOrder(BaseMember* LM, BaseMember* RM);
};


#endif //TESTSYSTEMS_ROOTGROUP_H
