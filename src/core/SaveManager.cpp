#include "SaveManager.hpp"


SaveManager::SaveManager() {
    std::filesystem::path saveDir = PATH;
    std::filesystem::create_directory(saveDir);
}


SaveManager::~SaveManager() {

}


void SaveManager::newSave(int gameID) {
    std::filesystem::path newSaveDir = PATH + std::to_string(gameID);
    std::filesystem::path playerDir = PATH + std::to_string(gameID) + "/Player";
    std::filesystem::path enemiesDir = PATH + std::to_string(gameID) + "/Enemies";
    std::filesystem::path cellsDir = PATH + std::to_string(gameID) + "/Cells";
    std::filesystem::create_directory(newSaveDir);
    std::filesystem::create_directory(playerDir);
    std::filesystem::create_directory(enemiesDir);
    std::filesystem::create_directory(cellsDir);
}