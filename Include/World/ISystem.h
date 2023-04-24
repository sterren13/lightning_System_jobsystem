//
// Created by sterr on 24/04/2023.
//

#ifndef TESTSYSTEMS_ISYSTEM_H
#define TESTSYSTEMS_ISYSTEM_H
#include "BaseSystem.h"
#include "Groups/IMember.hpp"

template<typename ParentGroup>
class ISystem : public BaseSystem, public IMember<ParentGroup> {
private:
    virtual void OnInit() = 0;
    virtual void OnStart() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnStop() = 0;
    virtual void OnDestroy() = 0;
};

#endif //TESTSYSTEMS_ISYSTEM_H
