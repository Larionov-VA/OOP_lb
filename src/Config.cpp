#include "Config.hpp"

/*
Выставлены настройки по умолчанию
Глобальные переменные!
*/
int GlobalGameConfig::fieldWidth = 25;
int GlobalGameConfig::fieldHeight = 25;
int GlobalGameConfig::gameLevel = 1;
const int GlobalGameConfig::enemyAttack = 5;
const int GlobalGameConfig::enemyHealth = 10;
GlobalGameConfig::difficulties GlobalGameConfig::difficulty = GlobalGameConfig::MEDIUM;

// std::vector<wchar_t> fieldChars;