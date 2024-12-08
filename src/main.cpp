#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <unordered_map>

using namespace std;

// Skill class
class Skill {
public:
    string name, type;
    int damage, maxTry, remainingTry;

    Skill(const string& name, const string& type, int damage, int maxTry)
        : name(name), type(type), damage(damage), maxTry(maxTry), remainingTry(maxTry) {}

    bool use() {
        if (remainingTry > 0) {
            --remainingTry;
            return true;
        }
        return false;
    }
};

// Pokemon class
class Pokemon {
public:
    string name, type;
    int maxHP, currentHP;
    vector<Skill> skills;

    Pokemon(const string& name, const string& type, int maxHP, const vector<Skill>& skills)
        : name(name), type(type), maxHP(maxHP), currentHP(maxHP), skills(skills) {}

    bool isAlive() const {
        return currentHP > 0;
    }

    void takeDamage(int damage) {
        currentHP = max(0, currentHP - damage);
    }
};

