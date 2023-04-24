//
// Created by sterr on 23/04/2023.
//

#ifndef TESTSYSTEMS_BASEMEMBER_HPP
#define TESTSYSTEMS_BASEMEMBER_HPP
#include <unordered_set>
#include <typeindex>
#include <string>

class BaseMember{
public:
    virtual ~BaseMember() = default;
    virtual void OnInit() = 0;
    virtual void OnStart() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnStop() = 0;
    virtual void OnDestroy() = 0;

    std::unordered_set<size_t> UpdateBefore = {};
    std::unordered_set<size_t> UpdateAfter = {};
    size_t MemberId = 0;
    std::string MemberName;
    bool IsEnabled = true;

    virtual void PrintHierarchy(size_t interval) = 0;
};

#endif //TESTSYSTEMS_BASEMEMBER_HPP
