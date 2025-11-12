#pragma once
#include <vector>
#include "ISaveManager.hpp"
#include "FileHandler.hpp"

class SavesTreeNode : public ISaveManager{
private:
    ISaveManager* wrappedObject;
    std::vector<ISaveManager*> childs{};
public:
    SavesTreeNode(ISaveManager* obj) : wrappedObject(obj) {};
    void addChild(ISaveManager* child) override {
        childs.push_back(child);
    }
    void saveState(int saveID) override {
        if (wrappedObject) {
            wrappedObject->saveState(saveID);
        }
        for (auto child : childs) {
            child->saveState(saveID);
        }
        FileHandler file{"savelog.txt", std::ios::out};
        // file.write(".\n");
    }
    void loadState(int loadID) override { (void)loadID; };
    char log() override { return '\0'; };
    std::vector<ISaveManager*> getNextNodes() {
        return childs;
    }
    std::vector<ISaveManager*> getChilds() override {
        return childs;
    }
    void clearChildren() {
        childs.clear();
    }
};