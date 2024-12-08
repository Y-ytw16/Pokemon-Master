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

// Type effectiveness chart
int getEffectiveness(const string& attackType, const string& defenderType) {
    static const unordered_map<string, unordered_map<string, int>> effectivenessMap = {
        {"Normal", { {"Ground", 0}, {"Electric", 0}, {"Water", 0}, {"Grass", 0}, {"Fire", 0} }},
        {"Ground", { {"Electric", 5}, {"Grass", -3}, {"Fire", 5} }},
        {"Electric", { {"Ground", -3}, {"Water", 5}, {"Grass", -3} }},
        {"Water", { {"Fire", 5}, {"Grass", -3}, {"Ground", 5} }},
        {"Grass", { {"Water", 5}, {"Ground", 5}, {"Fire", -3} }},
        {"Fire", { {"Grass", 5}, {"Water", -3}, {"Ground", -3} }}
    };

    if (effectivenessMap.count(attackType) && effectivenessMap.at(attackType).count(defenderType)) {
        return effectivenessMap.at(attackType).at(defenderType);
    }
    return 0; // Default effectiveness
}

// Display battle page
void displayBattlePage(const Pokemon& pokemon1, const Pokemon& pokemon2, const string& latestSkill1, const string& latestSkill2, const string& effectiveness1, const string& effectiveness2, bool isPlayer1Turn) {
    cout << "+-------------------------------------------------------------+\n";
    cout << "| 2024-02 Object-Oriented Programming Pokemon Master          |\n";
    cout << "+------------------------------+------------------------------+\n";
    cout << "| " << setw(29) << left << pokemon1.name + (isPlayer1Turn ? " (*)" : "")
         << "| " << setw(29) << left << pokemon2.name + (!isPlayer1Turn ? " (*)" : "") << "|\n";
    cout << "| Type: " << setw(23) << left << pokemon1.type
         << "| Type: " << setw(23) << left << pokemon2.type << "|\n";
    cout << "| HP: " << setw(25) << left << pokemon1.currentHP
         << "| HP: " << setw(25) << left << pokemon2.currentHP << "|\n";
    cout << "+------------------------------+------------------------------+\n";
    cout << "| Latest Skill: " << setw(15) << left << latestSkill1
         << "| Latest Skill: " << setw(15) << left << latestSkill2 << "|\n";
    cout << "| " << setw(29) << left << effectiveness1
         << "| " << setw(29) << left << effectiveness2 << "|\n";
    cout << "+------------------------------+------------------------------+\n";

    for (int i = 0; i < 4; ++i) {
        displaySkillInfo(i, pokemon1.skills[i], pokemon2.skills[i]);
    }
    cout << "+------------------------------+------------------------------+\n";
}

// Choose a Pokémon
pair<Pokemon, Pokemon> choosePokemon(const vector<Pokemon>& pokemons) {
    int choice1, choice2;
    cout << "Choose a Pokemon(0~4): ";
    cin >> choice1;
    cout << "Choose a Pokemon(0~4): ";
    cin >> choice2;

    if (choice1 == choice2) {
        cout << "You have to choose Pokemons different from each other." << endl;
        exit(0);
    }

    return {pokemons[choice1], pokemons[choice2]};
}

int main() {
    // Initialize Pokémon and skills
    vector<Pokemon> pokemons = {
        {"Pikachu", "Electric", 35, { {"Tackle", "Normal", 4, 5}, {"Grass Knot", "Grass", 8, 5}, {"Thunderbolt", "Electric", 10, 5}, {"Megabolt", "Electric", 15, 3} }},
        {"Dratini", "Water", 41, { {"Wrap", "Normal", 4, 10}, {"Aqua Tail", "Water", 3, 5}, {"Water Pulse", "Water", 13, 2}, {"Hyper Beam", "Normal", 20, 1} }},
        {"Eevee", "Normal", 55, { {"Tackle", "Normal", 4, 5}, {"Sand Attack", "Ground", 8, 3}, {"Bite", "Normal", 12, 3}, {"Rain Dance", "Water", 15, 1} }},
        {"Charmander", "Fire", 39, { {"Tackle", "Normal", 4, 5}, {"Flamethrower", "Fire", 11, 5}, {"Dig", "Ground", 7, 5}, {"Heat Wave", "Fire", 14, 5} }},
        {"Palkia", "Water", 90, { {"Hydro Pump", "Water", 12, 10}, {"Earth Power", "Ground", 15, 10}, {"Surf", "Water", 13, 10}, {"Spatial Rend", "Normal", 30, 10} }}
    };

    // Choose Pokémon
    auto chosenPokemons = choosePokemon(pokemons);
    Pokemon& pokemon1 = chosenPokemons.first;
    Pokemon& pokemon2 = chosenPokemons.second;

    // Start battle
    battle(pokemon1, pokemon2);

    return 0;
}

