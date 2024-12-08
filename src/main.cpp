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


