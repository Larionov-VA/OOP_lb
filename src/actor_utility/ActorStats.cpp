#include "ActorStats.hpp"


Stats::Stats(long long prevLevelUp, long long experience, long long levelUp, int level) {
    this->prevLevelUpExperience = prevLevelUp;
    this->currentExperience = experience;
    this->levelUpExperience = levelUp;
    this->level = level;
}


Stats::Stats() : Stats(0, 0, 100, 1) {}


Stats::~Stats() {}


bool Stats::addExperience(int experience) {
    this->currentExperience += experience;
    if (this->currentExperience >= this->levelUpExperience) {
        this->level++;
        this->prevLevelUpExperience = this->levelUpExperience;
        this->levelUpExperience = this->levelUpExperience * this->level / 2 + 50;
        return true;
    }
    else {
        return false;
    }
}


int Stats::getLevel() {
    return this->level;
}


std::vector<long long> Stats::getExperience() {
    return {this->prevLevelUpExperience, this->currentExperience, this->levelUpExperience};
}