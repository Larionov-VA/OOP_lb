#pragma once
#include <vector>
#include <string>

struct GlobalGameConfig {
    static int fieldWidth;
    static int fieldHeight;
    static int gameLevel;
    static enum difficulties {
        EASY, MEDIUM, HARD
    } difficulty;
    const static int enemyAttack;
    const static int enemyHealth;
};


typedef struct {
    int playerMaxHealth;
    int playerHealth;
    int playerAttack;
    int playerIntelligence;
    int playerDexterity;
    int playerStrength;
    std::string playerWeapon;
    std::string playerDebaff;
}PlayerData;


extern std::vector<wchar_t> fieldChars;