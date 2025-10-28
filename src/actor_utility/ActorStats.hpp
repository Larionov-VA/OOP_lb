#pragma once
#include <vector>


class Stats {
private:
    long long prevLevelUpExperience;
    long long currentExperience;
    long long levelUpExperience;
    int level;
    bool levelIncreased;
public:
    Stats();
    Stats(long long prevLevelUp, long long experience, long long levelUp, int level);
    void addExperience(int experience);
    int getLevel();
    bool checkAndSwitchLevelIncreased();
    std::vector<long long> getExperience();
    ~Stats();
};