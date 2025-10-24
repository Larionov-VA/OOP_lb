#pragma once
#include <vector>
#include <string>
#define DEFAULT_ATTACK 5
#define DEFAULT_HEALTH 100


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



typedef struct {
    int enemyMaxHealth;
    int enemyHealth;
    int enemyAttack;
}EnemyData;