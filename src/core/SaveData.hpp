#pragma once

// struct AttributesSaveData;
// struct AttackSaveData;
// struct EquipmentSaveData;
// struct HandSaveData;
// struct StatsSaveData;
// struct HealthSaveData;
// struct CellsSaveData;
// struct BarrackSaveData;
// struct TowerSaveData;
// struct PlayerSaveData;
// struct EnemySaveData;
// struct EntitySaveData;
// struct FieldSaveData;


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

struct HandSaveData {
    int itemInHand;
    int countOfFirstSpell;
    int countOfSecondSpell;
    int countOfThirdpell;
    int countOfFourSpell;
    int countOfFiveSpell;
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
}

struct CellStateSaveData {
    int haveConstState;
    StateSaveData constState;
    int haveTempState;
    StateSaveData tempState;
    int avaible;
}

struct CellsSaveData {
    CellStateSaveData stateData;
    int index;
    int X;
    int Y;
};

struct BarrackSaveData {
    HealthSaveData barracksHealth;
    int spawnPeriod;
    int barracksLevel;
    int counter;
};

struct TowerSaveData {
    HealthSaveData towerHealth;
    DirectDamageSpell towerSpell;
    int attackPeriod;
    int attackCooldown;
    int towerlevel;
};

struct EnemySaveData {
    int data;
};


struct PlayerSaveData {
    AttributesSaveData playerAttributes;
    AttackSaveData playerAttack;
    EquipmentSaveData playerEquipment;
    HandSaveData playerHand;
    StatsSaveData playerStats;
    HealthSaveData playerHealth;
    bool slowed;
};


struct EntitySaveData {
    PlayerSaveData playerData;
    EnemySaveData enemyData;
    BarrackSaveData barrackData;
    TowerSaveData towerData;
};


struct FieldSaveData {
    EntitySaveData entityData;
    CellsSaveData cellsData;
    int widthField;
    int heightField;
    int gameLevel;
    int gameTurn;
};


struct SaveData {
    FieldSaveData fieldData;
    int gameID;
};