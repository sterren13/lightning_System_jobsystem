//
// Created by sterr on 23/04/2023.
//

#ifndef TESTSYSTEMS_GROUPTRAITS_HPP
#define TESTSYSTEMS_GROUPTRAITS_HPP
#include <unordered_set>
#include <string>

enum class GroupParallelJobSystem {
    Enabled,
    Disabled
};

class GroupTraits {
public:
    static constexpr GroupParallelJobSystem GroupParallelJobSystem = GroupParallelJobSystem::Disabled;
    static constexpr const char* GroupName = "";
    static constexpr const size_t GroupId = 0;
    static constexpr const size_t Before[] = {0};
    static constexpr const size_t After[] = {0};
};

#endif //TESTSYSTEMS_GROUPTRAITS_HPP
