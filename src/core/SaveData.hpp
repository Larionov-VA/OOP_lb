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
    int data;
};

struct AttackSaveData {
    int data;
};

struct EquipmentSaveData {
    int data;
};

struct HandSaveData {
    int data;
};

struct StatsSaveData {
    int data;
};

struct HealthSaveData {
    int data;
};

struct CellsSaveData {
    int data;
};

struct BarrackSaveData {
    int data;
};

struct TowerSaveData {
    int data;
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