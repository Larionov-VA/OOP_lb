#pragma once
#include <vector>

struct AttributesSaveData {
    int intelligence;
    int dexterity;
    int strength;
};


struct AttackSaveData {
    int attack;
};


struct EquipmentSaveData {
    int currentWeapon;
    int meleeWeaponMulti;
    int rangeWeaponMulti;
};


struct SpellSaveData {
    int countOfItem;
    int powerOfSpell;
};


struct AreaDamageSpellSaveData : SpellSaveData {
    int baseDamage;
    int baseDistance;
};


struct DirectDamageSpellSaveData : SpellSaveData {
    int baseDamage;
    int baseDistance;
};


struct UpdateSpellSaveData : SpellSaveData {

};


struct TrapSpellSaveData : SpellSaveData {
    int trapLevel;
    int trapDamage;
};


struct HandSaveData {
    int itemInHand; //0-3 Area, Direct, Update, Trap
    struct AreaDamageSpellSaveData areaSpell;
    struct DirectDamageSpellSaveData directSpell;
    struct UpdateSpellSaveData updateSpell;
    struct TrapSpellSaveData trapSpell;
    int powerUp;
    int maxSize;
    int currentSize;
};


struct StatsSaveData {
    long long prevLevelUpExperience;
    long long currentExperience;
    long long levelUpExperience;
    int level;
    bool levelIncreased;
};


struct HealthSaveData {
    int currentHealth;
    int maxHealth;
};


struct StateSaveData {
    int durationOfState;
    int damage;
    char stateSymbol;
};


struct CellStateSaveData {
    int haveConstState;
    struct StateSaveData constState;
    int haveTempState;
    struct StateSaveData tempState;
    int avaible;
};


struct CellSaveData {
    struct CellStateSaveData stateData;
    int index;
    int X;
    int Y;
};


struct BarrackSaveData {
    struct HealthSaveData barracksHealth;
    int spawnPeriod;
    int barracksLevel;
    int counter;
};


struct TowerSaveData {
    struct HealthSaveData towerHealth;
    struct DirectDamageSpellSaveData towerSpell;
    int attackPeriod;
    int attackCooldown;
    int towerlevel;
};


struct EnemySaveData {
    struct AttackSaveData enemyAttack;
    struct HealthSaveData playerHealth;
    int enemyLevel;
    bool iterative;
};


struct PlayerSaveData {
    struct AttributesSaveData playerAttributes;
    struct AttackSaveData playerAttack;
    struct EquipmentSaveData playerEquipment;
    struct HandSaveData playerHand;
    struct StatsSaveData playerStats;
    struct HealthSaveData playerHealth;
    bool slowed;
};


struct EntitySaveData {
    struct PlayerSaveData playerData;
    std::vector<struct EnemySaveData> enemyData;
    struct BarrackSaveData barrackData;
    struct TowerSaveData towerData;
};


struct FieldSaveData {
    struct EntitySaveData entityData;
    std::vector<struct CellSaveData> cellsData;
    int widthField;
    int heightField;
    int gameLevel;
    int gameTurn;
};


struct SaveData {
    struct FieldSaveData fieldData;
    int gameID;
};
