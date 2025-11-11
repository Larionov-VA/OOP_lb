#include "SaveManager.hpp"


SaveManager::SaveManager() {
    std::filesystem::path saveDir = SAVES_PATH;
    std::filesystem::create_directory(saveDir);
}


void SaveManager::createDirTree(int gameID) {
    std::filesystem::path newSaveDir = SAVES_PATH + std::to_string(gameID);
    std::filesystem::path gameDir = SAVES_PATH + std::to_string(gameID) + "/Game";
    std::filesystem::path gameFieldDir = gameDir.generic_string() + "/GameField";
    std::filesystem::path entityManagerDir = gameFieldDir.generic_string() + "/EntityManager";
    std::filesystem::path entityDir = entityManagerDir.generic_string() + "/Entities";
    std::filesystem::path fieldCellsDir = gameFieldDir.generic_string() + "/FieldCells";
    std::filesystem::path cellDir = fieldCellsDir.generic_string() + "/Cells";
    std::filesystem::create_directory(newSaveDir);
    std::filesystem::create_directory(gameDir);
    std::filesystem::create_directory(gameFieldDir);
    std::filesystem::create_directory(entityManagerDir);
    std::filesystem::create_directory(entityDir);
    std::filesystem::create_directory(fieldCellsDir);
    std::filesystem::create_directory(cellDir);
}


// void SaveManager::saveRecursive(int gameID, ISaveManager* headOfTree) {
//     if (headOfTree) {
//         FileHandler flog{"saverlog.txt", std::ios::app};
//         flog.write("gameID: " + std::to_string(gameID) + '\n');
//         flog.write(std::to_string(headOfTree->log()) + '\n');
//         headOfTree->saveState(gameID);
//         auto vecChilds = headOfTree->getChilds();
//         for (auto child : vecChilds) {
//             saveRecursive(gameID, child);
//         }
//     }
// }


void SaveManager::newSave(int gameID) {
    createDirTree(gameID);
    // saveRecursive(gameID, headOfTree);
}