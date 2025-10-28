#pragma once
#include <vector>
#include <string>
#include "EntityManager.hpp"
#include "FieldCell.hpp"
#include "../entities/Entity.hpp"
#define DEFAULT_ATTACK 5
#define DEFAULT_HEALTH 100


struct GlobalGameConfig {
    static int fieldWidth;
    static int fieldHeight;
    static int gameLevel;
    enum difficulties {
        DEAFAULT, EASY, MEDIUM, HARD
    };
    static int difficulty;
    const static int enemyAttack;
    const static int enemyHealth;
    const static int barracksHealth;
};


typedef struct {
    long long playerPrevLevelUpExperience;
    long long playerCurrentExperience;
    long long playerLevelUpExperience;
    int playerLevel;
    bool levelIncreased;
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