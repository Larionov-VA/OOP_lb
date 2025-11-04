#include <filesystem>
#include "FileHandler.hpp"
#define PATH "../saves/"

class SaveManager {
private:

public:
    void newSave(int gameID);
    SaveManager();
    ~SaveManager();
};