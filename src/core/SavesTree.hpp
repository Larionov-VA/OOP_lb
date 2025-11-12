#pragma once
#include <vector>
// #include "ISaveManager.hpp"
// #include "FileHandler.hpp"

class SavesTreeNode {
private:
    // ISaveManager* wrappedObject;
    std::vector<ISaveManager*> childs{};
public:
    // SavesTreeNode(ISaveManager* obj) : wrappedObject(obj) {};
    void addChild(ISaveManager* child) {
        childs.push_back(child);
    }
    // void saveState(int saveID) override { (void)saveID; };
    // void loadState(int loadID) override { (void)loadID; };
    // char log() override { return '\0'; };
    // std::vector<ISaveManager*> getNextNodes() {
    //     return childs;
    // }
    std::vector<ISaveManager*> getChilds() {
        return childs;
    }
    void clearChildren() {
        childs.clear();
    }
};