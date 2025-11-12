#pragma once
#include <vector>

class ISaveManager {
public:
    virtual void addChild(ISaveManager* child) = 0;
    virtual void saveState(int saveID) = 0;
    virtual void loadState(int loadID) = 0;
    // virtual char log() = 0;
    virtual std::vector<ISaveManager*> getChilds() = 0;
    virtual ~ISaveManager() = default;
};