#pragma once
struct IGameController {
    virtual void startGame() = 0;
    virtual bool performAnAction(char playerAction) = 0;
    // virtual void startGame() = 0;
    virtual void stopGame() = 0;
    virtual ~IGameController() = default;
};