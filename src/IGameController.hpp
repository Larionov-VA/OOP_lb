#pragma once
struct IGameController {
    virtual void startGame() = 0;
    virtual void stopGame() = 0;
    virtual ~IGameController() = default;
};