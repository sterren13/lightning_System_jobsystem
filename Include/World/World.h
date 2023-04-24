//
// Created by sterr on 24/04/2023.
//

#ifndef TESTSYSTEMS_WORLD_H
#define TESTSYSTEMS_WORLD_H
#include <unordered_map>
#include <memory>
#include <typeindex>

#include "BaseSystem.h"
#include "Groups/RootGroup.h"

class World {
public:
    World();
    ~World();

    void OnInit();

    void OnStart();

    void OnUpdate();

    void OnStop();

    void OnDestroy();

    void PrintGroupTree();

    template<typename T, typename ... Args>
    void CreateSystem(Args&& ... args){
        static_assert(std::is_base_of<BaseSystem, T>::value, "T must inherit from BaseSystem");
        systems[std::type_index(typeid(T))] = std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename Interface, typename System, typename ... Args>
    void CreateSystem(Args&& ... args){
        static_assert(std::is_base_of<BaseSystem, System>::value, "System must inherit from BaseSystem");
        systems[std::type_index(typeid(Interface))] = std::make_shared<System>(std::forward<Args>(args)...);
    }

    template<typename T>
    void DestroySystem(){
        static_assert(std::is_base_of<BaseSystem, T>::value, "T must inherit from BaseSystem");
        // TODO assert if system is not found
        systems.erase(std::type_index(typeid(T)));
    }

    template<typename T>
    std::shared_ptr<BaseSystem> GetSystem(){
        static_assert(std::is_base_of<BaseSystem, T>::value, "T must inherit from BaseSystem");
        // TODO assert if system is not found
        return systems[std::type_index(typeid(T))];
    }

private:
    RootGroup rootGroup;
    std::unordered_map<std::type_index, std::shared_ptr<BaseSystem>> systems;
};


#endif //TESTSYSTEMS_WORLD_H
