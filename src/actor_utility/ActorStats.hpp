#pragma once
#include <vector>


class Stats {
private:
    long long prevLevelUpExperience;
    long long currentExperience;
    long long levelUpExperience;
    int level;
public:
    Stats();
    Stats(long long prevLevelUp, long long experience, long long levelUp, int level);
    bool addExperience(int experience);
    int getLevel();
    std::vector<long long> getExperience();
    ~Stats();
};