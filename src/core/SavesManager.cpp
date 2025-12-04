#include "SavesManager.hpp"


SavesManager::SavesManager() {
    try {
        FileHandler file("SavesManager_log", std::ios::out);
        std::string path = "../core/";
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            std::cout << entry.path() << std::endl;
            file.write(entry.path());
            file.write("\n");
        }
    } catch(...) {

    }
}


SavesManager::~SavesManager() {

}