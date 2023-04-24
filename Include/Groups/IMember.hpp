//
// Created by sterr on 23/04/2023.
//

#ifndef TESTSYSTEMS_IMEMBER_HPP
#define TESTSYSTEMS_IMEMBER_HPP
#include <iostream>
#include "BaseMember.hpp"
#include "Group.hpp"

template<class ParentGroup>
class IMember : public BaseMember {
public:
    void RegisterMember() {
        ParentGroup::AddMember(this);
    }

    ~IMember() {
        ParentGroup::RemoveMember(this);
    }

    void PrintHierarchy(size_t interval) override {
        for (size_t i = 0; i < interval; i++)
            std::cout << "  ";
        std::cout << "| IMember: " << MemberName << ", ID: " << MemberId << std::endl;
    }

    virtual void OnInit() = 0;
    virtual void OnStart() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnStop() = 0;
    virtual void OnDestroy() = 0;
};

#endif //TESTSYSTEMS_IMEMBER_HPP
