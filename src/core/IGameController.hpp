#pragma once
#include "Config.hpp"

struct IGameController {
    virtual void startGame() = 0;
    virtual void nextLevel() = 0;
    virtual void playerLevelUp(char attribute) = 0;
    virtual bool performAnAction(char playerAction) = 0;
    virtual std::shared_ptr<PlayerData> getPlayerData() = 0;
    virtual std::vector<EnemyData> getEnemyData() = 0;
    virtual std::vector<wchar_t> getFieldData() = 0;
    virtual void stopGame() = 0;
    virtual ~IGameController() = default;
};